struct updater_ctx {
	char *link;
	int socket;
	unsigned int size;
	unsigned int bytes;
	unsigned int checksum;
	char *version;
};

extern int updater_init_ctx(struct updater_ctx *ctx, char *repository, char *fpath);
extern void updater_free_ctx(struct updater_ctx *ctx);
extern int updater_read_bytes(struct updater_ctx *ctx, unsigned char *buf, int size);
