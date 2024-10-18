#ifndef IDS_HANDLER_H
#define IDS_HANDLER_H

enum {ID_NOT_READY = 0, ID_INCOMPLETE = 1, ID_COMPLETE = 2 , DATA = 3};

bool id_parse(uint8_t ch);
void id_xmit(uint8_t ch, uint8_t page);

void data_parse(uint8_t ch);
void data_xmit(uint8_t ch, uint8_t page);

void ids_init(uint8_t ch);
void on_id_frame_receipt(uint8_t ch);
void on_id_frame_xmit(uint8_t ch);

void copy_buffer_data(uint8_t * source, uint8_t * destination, uint8_t size, bool string);
void invalidate_bytes_page(uint8_t * buffer, uint8_t mask);

#endif