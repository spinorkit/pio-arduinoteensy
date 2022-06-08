/***********************************************************************************
 *  SnoozeUSBSerial.h
 *  Teensy 3.6
 *
 * Purpose: USBSerial Driver
 *
 **********************************************************************************/
#if defined(__MK66FX1M0__)

#include "SnoozeUSBSerial.h"
#include "usb_dev.h"
/*******************************************************************************
 *  Stream virtual functions
 *******************************************************************************/
size_t SnoozeUSBSerial::write( uint8_t b ) {
    return usb_serial_putchar( b );
}

size_t SnoozeUSBSerial::write( const uint8_t *buffer, size_t size ) {
    size_t count = 0;
    if ( Serial ) {
        if ( strlen( print_buffer ) ) {
            char *buf = print_buffer;
            int pb_size  = strlen( print_buffer );
            while ( pb_size-- ) count += write( *buf++ );
            flush( );
            memset( print_buffer, 0, strlen( print_buffer ) );
        }
        if ( buffer == nullptr ) return 0;
        while ( size-- ) count += write( *buffer++ );
        return count;
    } else {
        int pb_size  = strlen( print_buffer );
        if ( pb_size + size > USB_SERIAL_BUFFER_SIZE ) return 0;
        strcat( print_buffer, ( const char * )buffer );
        write( 0x00 );
    }
    return count;
}

int SnoozeUSBSerial::availableForWrite( void ) {
    return usb_serial_write_buffer_free( );
}

int SnoozeUSBSerial::available( ) {
    return usb_serial_available( );
}

void SnoozeUSBSerial::flush( void ) {
    usb_serial_flush_output( );
}

int SnoozeUSBSerial::read( ) {
    return usb_serial_getchar( );
}

int SnoozeUSBSerial::peek( ) {
    return usb_serial_peekchar( );
}
/*******************************************************************************
 *  Sets the usb_configuration, usb_cdc_line_rtsdtr and usb_cdc_line_rtsdtr_millis
 *  to initial state so while(!Serial) works after sleeping. Still buggy...
 *******************************************************************************/
void SnoozeUSBSerial::disableDriver( uint8_t mode ) {
#if F_CPU >= 20000000 && defined(USB_SERIAL)
    if (mode == 0) return;
    //usb_configuration = 0;
    usb_cdc_line_rtsdtr = 0;
    usb_cdc_line_rtsdtr_millis = systick_millis_count;
    if ( mode == 1 ) {
        SIM_SCGC4 |= SIM_SCGC4_USBOTG;// enable USB clock
        USB0_USBCTRL &= ~USB_USBCTRL_SUSP;// suspended State
        USB0_CTL |= USB_CTL_USBENSOFEN;// enable USB
        while ( ( USB0_CTL & USB_CTL_USBENSOFEN ) != 0x01 );
#if defined(__MK66FX1M0__) && ((F_CPU == 256000000) || (F_CPU == 216000000) || (F_CPU == 180000000))
        SIM_SOPT2 |= SIM_SOPT2_IRC48SEL;
#endif
    }
#endif
}
/*******************************************************************************
 *  Turns off usb clock if using 'sleep'.
 *******************************************************************************/
void SnoozeUSBSerial::enableDriver( uint8_t mode ) {
#if F_CPU >= 20000000 && defined(USB_SERIAL)
    if (mode == 0) return;
    if ( mode == 1 ) {
        USB0_USBCTRL |= USB_USBCTRL_SUSP;// suspended State
        USB0_CTL &= ~USB_CTL_USBENSOFEN;// disable USB
        while ( ( USB0_CTL & USB_CTL_USBENSOFEN ) != 0x00 );
#if defined(__MK66FX1M0__) && ((F_CPU == 256000000) || (F_CPU == 216000000) || (F_CPU == 180000000))
        SIM_SOPT2 &= ~SIM_SOPT2_IRC48SEL;
#endif
        SIM_SCGC4 &= ~SIM_SCGC4_USBOTG;// disable USB clock
    }
#endif
}
/*******************************************************************************
 *  not used
 *******************************************************************************/
void SnoozeUSBSerial::clearIsrFlags( uint32_t ipsr ) {
    
}
/*******************************************************************************
 *  not used
 *******************************************************************************/
void SnoozeUSBSerial::isr( void ) {

}
#endif /* __MK66FX1M0__ */
