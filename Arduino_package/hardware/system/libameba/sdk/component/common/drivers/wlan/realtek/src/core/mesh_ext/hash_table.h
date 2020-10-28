#ifndef HASH_TABLE

#define HASH_TABLE

#define HASH_SEARCH(table, id)				table->search_entry(table, id)
#define HASH_INSERT(table, id, data)		table->insert_entry(table, id, data)
#define HASH_DELETE(table, id)				table->delete_entry(table, id)
#define HASH_TRAVERSE(table, p)				table->traverse_table(table, p)

enum {HASH_TABLE_FAILED = -1, HASH_TABLE_SUCCEEDED};

struct hash_table_entry
{
	unsigned char dirty;
	void *hash_id;
	void *data;
};

struct hash_table
{
	struct hash_table_entry *entry_array;
	unsigned long table_size_power;	//the ith power of 2
	unsigned long hash_id_length;	//byte
	unsigned long data_length;	//byte
	unsigned long (*hash_function) (struct hash_table*, void*);
	void* (*search_entry) (struct hash_table*, void*);
	unsigned long (*insert_entry) (struct hash_table*, void*, void*);
	unsigned long (*delete_entry) (struct hash_table*, void*);
	void (*traverse_table) (struct hash_table*, void*);
};

extern long remove_hash_table (struct hash_table*);

extern long init_hash_table (struct hash_table*, unsigned long, unsigned long, unsigned long, void*, void*, void*, void*, void*);

extern long crc_hashing (struct hash_table*, void*);
//pepsi
extern long PU_hashing (struct hash_table*, void*);

extern void* search_default (struct hash_table*, void*);

extern long insert_default (struct hash_table*, void*, void*);
//pepsi
extern long insert_PUtbl (struct hash_table*, void*, void*);

extern long delete_default (struct hash_table*, void*);

extern void traverse_default (struct hash_table*, void*);

#endif
