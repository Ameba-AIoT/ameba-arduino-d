/////////////////////////////////////////////////
//
// kv , key-value pair
//
/////////////////////////////////////////////////

#include "lfs.h"
#include "kv.h"
#include "littlefs_adapter.h"

extern struct lfs_config  g_lfs_cfg;
int g_kv_init_done = 0;

kv_reg_ptr kv_encrypt = 0;
kv_reg_ptr kv_decrypt = 0;

void rt_kv_init(void)
{
	lfs_t lfs;
	
	if (lfs_mount(&lfs, &g_lfs_cfg)) {
		printf("mount failed, need to format!!!!!\r\n");
		lfs_format(&lfs, &g_lfs_cfg);
		lfs_mount(&lfs, &g_lfs_cfg);
	}
	if (lfs_mkdir(&lfs, "KV"))
		printf("mkdir kv failed, mayed already exist!!!!\r\n");
	lfs_unmount(&lfs);

}

int32_t rt_kv_set(const char *key, const void *val, int32_t len)
{
	lfs_t lfs;
	lfs_file_t file;
	int32_t res;
	char path[MAX_KEY_LENGTH];
	
	lfs_mount(&lfs, &g_lfs_cfg);
	sprintf(path, "KV/%s", key);
	lfs_file_open(&lfs, &file, path, LFS_O_WRONLY | LFS_O_CREAT | LFS_O_TRUNC);

	if (kv_encrypt) {
		unsigned char *aesencsw;
		unsigned char iv_len = 16;
		unsigned short msglen = (len / iv_len + 1) * iv_len;

		aesencsw = malloc(msglen * sizeof(unsigned char));
		kv_encrypt((void *)val, aesencsw, len);
		res = lfs_file_write(&lfs, &file, aesencsw, msglen);
		free(aesencsw);
		if (res != msglen) {
			printf("[%s] write failed, err is %d!!!", __FUNCTION__, res);
		} else {
			res = len;
		}
	} else {
		res = lfs_file_write(&lfs, &file, val, len);

		if (res != len) {
			printf("[%s] write failed, err is %d!!!", __FUNCTION__, res);
		}
	}

	lfs_file_close(&lfs, &file);
	lfs_unmount(&lfs);
	return res;
}

int32_t rt_kv_get(const char *key, void *buffer, int32_t len)
{
	lfs_t lfs;
	lfs_file_t file;
	int32_t res;
	char path[MAX_KEY_LENGTH];
	
	lfs_mount(&lfs, &g_lfs_cfg);
	sprintf(path, "KV/%s", key);
	res = lfs_file_open(&lfs, &file, path, LFS_O_RDONLY);
	if (res) {
		printf("[%s] open file failed , err is %d!!!", __FUNCTION__, res);
		goto exit;
	}
	if (kv_decrypt) {
		unsigned char *aesencsw;
		unsigned char iv_len = 16;
		unsigned short msglen = (len / iv_len + 1) * iv_len;

		aesencsw = malloc(msglen * sizeof(unsigned char));
		res = lfs_file_read(&lfs, &file, aesencsw, msglen);
		kv_decrypt(aesencsw, buffer, len);
		free(aesencsw);
		if (res != msglen) {
			printf("[%s] read failed, err is %d!!!", __FUNCTION__, res);
		} else {
			res = len;
		}
	} else {
		res = lfs_file_read(&lfs, &file, buffer, len);

		if (res != len) {
			printf("[%s] read failed, err is %d!!!", __FUNCTION__, res);
		}
	}
	lfs_file_close(&lfs, &file);
exit:
	lfs_unmount(&lfs);
	return res;
}

int32_t rt_kv_delete(const char *key)
{
	lfs_t lfs;
	int32_t res;
	char path[MAX_KEY_LENGTH];

	lfs_mount(&lfs, &g_lfs_cfg);
	sprintf(path, "KV/%s", key);
	res = lfs_remove(&lfs, path);
	return res;
}