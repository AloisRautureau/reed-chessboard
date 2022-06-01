use serial_communication::handle_messages;
use serialport::SerialPort;
use std::thread;
use std::sync::mpsc;

pub mod serial_communication;
pub mod move_parser;

pub struct Chessboard {
    move_stack: mpsc::Receiver<String>
}
impl Chessboard {
    pub fn new() -> Self {
        let (transmitter, move_stack) = mpsc::channel();

        thread::spawn(move || {
            run(transmitter)
        });

        Chessboard {
            move_stack
        }
    }

    /// Blocks the thread it is called from until the driver
    /// receives a move
    pub fn wait_for_move(&mut self) -> String {
        self.move_stack.recv().unwrap()
    }

    /// Tries to get a move if one is available, non-blocking
    pub fn get_move(&mut self) -> Option<String> {
        if let Ok(mv) = self.move_stack.try_recv() {
            Some(mv)
        } else {
            None
        }
    }
}

/// The main loop function
fn run(move_stack: mpsc::Sender<String>) {
    let mut controller_port: Option<Box<dyn SerialPort>> = None;

    // While looping, we either :
    // - Have a controller, then we handle messages comming from it and consider
    //   it disconnected if we run into and error
    // - Don't have a controller, then we wait for one
    loop {
        controller_port = match controller_port {
            Some(mut port) => {
                if let Err(_) = handle_messages(&mut port, &move_stack) { 
                    println!("lost connection");
                    None 
                } else { 
                    Some(port) 
                }
            }
            None => Some(wait_for_controller()),
        }
    }
}

fn wait_for_controller() -> Box<dyn SerialPort> {
    println!("waiting for a compatible controller...");
    loop {
        if let Ok(p) = serial_communication::connect() {
            println!("connected!");
            break p;
        }
    }
}