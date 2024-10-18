#ifndef J3072_HANDLER_H
#define J3072_HANDLER_H

enum {CERT_NOT_READY = 0, CERT_INCOMPLETE = 1, CERT_COMPLETE = 2, SUNSPEC = 3};

bool cert_parse(uint8_t ch);
void cert_xmit(uint8_t ch, uint8_t page);

void j3072_init(uint8_t ch);
void on_j3072_frame_receipt(uint8_t ch);
void on_j3072_frame_xmit(uint8_t ch);


#endif