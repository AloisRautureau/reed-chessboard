use rcd::serial_communication::handle_messages;

fn main() {
    let mut port = rcd::init();
    loop {
        handle_messages(&mut port);
    }
}
