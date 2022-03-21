use std::time::Duration;

use serialport::{Error, ErrorKind, SerialPort};
use crate::move_parser;

/// Tries connecting to a controller.
/// Returns a SerialPort to interface with if it can find one,
/// or an Error.
pub fn connect() -> Result<Box<dyn SerialPort>, Error> {
    let ports = serialport::available_ports()?;

    for port_info in ports {
        println!("probing port {}", port_info.port_name);
        match probe(&port_info.port_name) {
            Some(p) => return Ok(p),
            None => ()
        }
    }

    Err(Error::new(ErrorKind::NoDevice, "No compatible serial device was found"))
}

/// Handles the last message received from the given serial port
pub fn handle_messages(port: &mut Box<dyn SerialPort>) {
    // The only possible message to receive as of now is just a move byte
    let mut message = read(port, 2);
    message.reverse();
    let move_bytes = message.iter().enumerate().fold(0, |acc: u16, (i, b)| { acc + ((*b as u16) << (i*8)) });

    if let Ok(mv) = move_parser::parse(move_bytes) {
        println!("reading move: {}", mv);
    }
}

/// Reads a given number of bytes from the serial port's standard output
/// # Arguments
/// * `port` - The port to read from 
/// * `bytes_count` - The number of bytes to read
fn read(port: &mut Box<dyn SerialPort>, bytes_count: usize) -> Vec<u8> {
    let mut buffer: Vec<u8> = vec![0; bytes_count];
    port.read(&mut buffer).expect("Could not read from the serial port");
    buffer
}

/// Checks if a given serial port is
/// a compatible serial device
fn probe(port_name: &str) -> Option<Box<dyn SerialPort>> {
    let mut port = match serialport::new(port_name, 115_200)
            .timeout(Duration::from_millis(10))
            .open() {
                Ok(p) => p,
                Err(_) => return None
            };

    let probe = "rcdprobe".as_bytes();
    if let Err(_) = port.write(probe) { return None }
    
    let buffer: Vec<u8> = read(&mut port, 5);

    if &buffer == "rccok".as_bytes() { Some(port) }
    else { None }
}