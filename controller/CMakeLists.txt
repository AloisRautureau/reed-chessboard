add_executable(reed_chessboard_controller
    main.c
    read.h
    bitboard.h
    move.h
    )

target_link_libraries(reed_chessboard_controller pico_stdlib)

pico_add_extra_outputs(reed_chessboard_controller)

pico_enable_stdio_usb(reed_chessboard_controller 1)