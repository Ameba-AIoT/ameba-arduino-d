#ifndef SNAPSHOT_TFTP_HANDLER_H
#define SNAPSHOT_TFTP_HANDLER_H

void jpeg_snapshot_create_tftp_thread();
void jpeg_snapshot_set_tftp_host_ip(char* addr_string);
char* jpeg_snapshot_get_tftp_host_ip();
void jpeg_snapshot_set_filename(char* file_name);
char* jpeg_snapshot_get_filename();

#endif /* SNAPSHOT_TFTP_HANDLER_H */