use serial_communication::handle_messages;
use serialport::SerialPort;

pub mod serial_communication;
pub mod move_parser;

const VERSION: &str = env!("CARGO_PKG_VERSION");

/// Initializes the driver and tries to find a compatible device
pub fn init() -> () {
    println!("rcd v{} by Aloïs 'baub' RAUTUREAU, 2022", VERSION);
}

/// The main loop function
pub fn run() -> () {
    let mut controller_port: Option<Box<dyn SerialPort>> = None;

    // While looping, we either :
    // - Have a controller, then we handle messages comming from it and consider
    //   it disconnected if we run into and error
    // - Don't have a controller, then we wait for one
    loop {
        controller_port = match controller_port {
            Some(mut port) => {
                if let Err(_) = handle_messages(&mut port) { 
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