use std::time::Duration;
use std::thread;

use serialport::{Error, ErrorKind, SerialPort};
use crate::move_parser;

#[repr(u8)]
pub enum DriverMessage {
    Syn = 0x01,
    ParsedMove = 0x10,
    IllegalMove = 0x11,
    Reset = 0xF0,
}

#[repr(u8)]
pub enum ControllerMessage {
    Ack = 0x01,
    Move = 0x10,
    State = 0x20,
}

/// Tries connecting to a controller.
/// Returns a SerialPort to interface with if it can find one,
/// or an Error.
pub fn connect() -> Result<Box<dyn SerialPort>, Error> {
    let ports = serialport::available_ports()?;

    for port_info in ports {
        match probe(&port_info.port_name) {
            Some(p) => return Ok(p),
            None => ()
        }
    }

    Err(Error::new(ErrorKind::NoDevice, "No compatible serial device was found"))
}

/// Handles the last message received from the given serial port
/// # Arguments
/// * `port` - The port to read from 
pub fn handle_messages(port: &mut Box<dyn SerialPort>) -> Result<(), serialport::Error> {
    if let Some(message) = read_byte(port) {
        if ControllerMessage::Move as u8 == message {
            thread::sleep(Duration::from_millis(5));
            let move_bytes = read_move_bytes(port).unwrap_or(0xFFFF);
            let move_string = match move_parser::parse(move_bytes) {
                Ok(mv) => {
                    port.write(&[DriverMessage::ParsedMove as u8])?;
                    mv
                }
                Err(_) => { port.write(&[DriverMessage::IllegalMove as u8])?; String::from("") }
            };
            println!("move {}", move_string);
        } else if ControllerMessage::Ack as u8 == message {
            println!("received a new ack")
        } else {
            println!("received an unknown command: {}", message)
        }
    }

    Ok(())
}

/// Reads the next byte in the serial port's standard output if
/// there is one
/// # Arguments
/// * `port` - The port to read from 
fn read_byte(port: &mut Box<dyn SerialPort>) -> Option<u8> {
    if let Some(bytes) = read_bytes(port, 1) {
        Some(*(bytes.first().unwrap()))
    } else {
        None
    }
}

/// Reads the next move bytes in the serial port's standard output
/// # Arguments
/// * `port` - The port to read from
fn read_move_bytes(port: &mut Box<dyn SerialPort>) -> Option<u16> {
    if let Some(bytes) = read_bytes(port, 2) {
        Some(bytes
            .iter()
            .enumerate()
            .fold(0, |acc, (i, b)| { acc + ((*b as u16) << (i*8)) })
        )
    } else {
        None
    }
}

/// Reads the next 8 bytes (corresponding to a bitboard) in the serial port's
/// standard output
/// # Arguments
/// * `port` - The port to read from
fn read_state_bytes(port: &mut Box<dyn SerialPort>) -> Option<u64> {
    if let Some(bytes) = read_bytes(port, 8) {
        Some(bytes
            .iter()
            .enumerate()
            .fold(0, |acc, (i, b)| { acc + ((*b as u64) << (i*8))} )
        )
    } else {
        None
    }
}

/// Reads a given number of bytes from the serial port's standard output
/// # Arguments
/// * `port` - The port to read from 
/// * `bytes_count` - The number of bytes to read
fn read_bytes(port: &mut Box<dyn SerialPort>, bytes_count: usize) -> Option<Vec<u8>> {
    let mut buffer: Vec<u8> = vec![0; bytes_count];
    if let Err(_) = port.read(&mut buffer) {
        None
    } else {
        Some(buffer)
    }
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

    if let Err(_) = port.write(&[DriverMessage::Syn as u8]) { return None }

    if read_byte(&mut port) == Some(ControllerMessage::Ack as u8) { Some(port) }
    else { None }
}