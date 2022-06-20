#ifndef IDS_HANDLER_H
#define IDS_HANDLER_H

enum {ID_INCOMPLETE = 0, ID_COMPLETE, DATA};

bool id_parse();
void id_xmit(uint8_t page);

void data_parse();
void data_xmit(uint8_t page);

void ids_init();
void on_id_frame_receipt();
void on_id_frame_xmit();


#endif