#ifdef OLED_ENABLE
bool oled_task_user(void) {
    // Host Keyboard Layer Status
    oled_write_P(PSTR("Layer: "), false);

    switch (get_highest_layer(layer_state)) {
        case 0:
            oled_write_P(PSTR("base\n"), false);
            break;
        case 1:
            oled_write_P(PSTR("num\n"), false);
            break;
        case 2:
            oled_write_P(PSTR("special\n"), false);
            break;
	case 3:
            oled_write_P(PSTR("fn\n"), false);
            break;
        default:
            // Or use the write_ln shortcut over adding '\n' to the end of your string
            oled_write_ln_P(PSTR("none??"), false);
    }
   
    return false;
}
#endif
