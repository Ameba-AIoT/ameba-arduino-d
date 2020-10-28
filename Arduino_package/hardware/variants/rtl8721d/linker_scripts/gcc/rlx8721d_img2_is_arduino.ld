ENTRY(Reset_Handler)

INCLUDE "rom_symbol_ns.txt"

MEMORY
{
	IROM (rx) : 				ORIGIN = 0x10100000, LENGTH = 0x1010A000 - 0x10100000	/* ROM: 40k */
	IROM_NS (rx) : 				ORIGIN = 0x1010A000, LENGTH = 0x10140000 - 0x1010A000	/* ROM: 216k */
	DROM_NS (rx) : 				ORIGIN = 0x101C0000, LENGTH = 0x101D4000 - 0x101C0000	/* ROM: 80k */
	DROM (rx) : 				ORIGIN = 0x101D4000, LENGTH = 0x101D8000 - 0x101D4000	/* ROM: 16k */
	ROMBSS_RAM_COM (rw) : 		ORIGIN = 0x10000000, LENGTH = 0x10001000 - 0x10000000	/* ROM BSS COMMON(S & NS both used) RAM: 4K */
	ROMBSS_RAM_NS (rw) : 		ORIGIN = 0x10001000, LENGTH = 0x10002000 - 0x10001000	/* ROM BSS NS RAM: 4K */
	RSVD_RAM_NS (rw) : 			ORIGIN = 0x10002000, LENGTH = 0x10004000 - 0x10002000	/* RSVD RAM: 8K */
	MSP_RAM_NS (rw) : 			ORIGIN = 0x10004000, LENGTH = 0x10005000 - 0x10004000	/* MSP_NS RAM: 4K */

	BD_RAM_NS (rwx) : 			ORIGIN = 0x10005000, LENGTH = 0x1007C000 - 0x10005000	/* MAIN RAM NS: 456K */
	ROMBSS_RAM_S (rwx)  : 		ORIGIN = 0x1007C000, LENGTH = 0x1007D000 - 0x1007C000	/* ROM BSS RAM S: 4K */
	BOOTLOADER_RAM_S (rwx)  : 	ORIGIN = 0x1007D000, LENGTH = 0x1007F000 - 0x1007D000	/* BOOT Loader RAM: 8K */
	MSP_RAM_S (rwx)  : 		ORIGIN = 0x1007F000, LENGTH = 0x10080000 - 0x1007F000	/* MSP_S RAM: 4k */
	EXTENTION_SRAM (rwx)  : 		ORIGIN = 0x100E0000, LENGTH = 0x10100000 - 0x100E0000	/* EXTENTION SRAM: 128k */

	PSRAM_NS (rwx)  : 		ORIGIN = 0x02000000 + 0x20, LENGTH = 0x02400000 - 0x02000000 - 0x20	/* PSRAM_NS: 4M */

	/* Flash */
	KM0_BOOT (rx)  :		ORIGIN = 0x08000000+0x20, LENGTH = 0x02000-0x20	/* XIPBOOT: 8k, 32 Bytes resvd for header*/
	BACKUP (r) :			ORIGIN = 0x08002000, LENGTH = 0x1000	/* BACKUP: 4K system data in flash */
	XIPSYS (r) :				ORIGIN = 0x08003000, LENGTH = 0x1000	/* XIPSYS: 4K system data in flash */
	KM4_BOOT (rx) :				ORIGIN = 0x08004000+0x20, LENGTH = 0x02000-0x20	/* XIPBOOT: 8k, 32 Bytes resvd for header*/
	KM0_IMG2 (rx) : 			ORIGIN = 0x0C000000+0x20, LENGTH = 0x02000000-0x20	/* KM0_IMG2: 32MB, 32 Bytes resvd for header, virtual address */
	KM4_IMG2 (rx) : 			ORIGIN = 0x0E000000+0x20, LENGTH = 0x02000000-0x20	/* KM4_IMG2 OTA1: 32MB, 32 Bytes resvd for header,  virtual address */
	BTRACE (rx) :				ORIGIN = 0x00800000, LENGTH = 0x00C00000 -0x00800000	/* Bluetooth Trace */

	/* KM0 RAM*/
	KM0_SRAM (rwx) : 			ORIGIN = 0x00080000, LENGTH = 0x00090000 - 0x00080000	/* KM0 SRAM: 64k */
	RETENTION_RAM (rwx) : 		ORIGIN = 0x000C0000, LENGTH = 0x000C0400 - 0x000C0000	/* KM0 Retention SRAM: 1k */
}

SECTIONS
{
	.rom.text : { } > IROM_NS
	.rom.rodata : { } > DROM_NS
	.hal.rom.bss : { } > ROMBSS_RAM_COM
	.hal.ns_rom.bss : { } > ROMBSS_RAM_NS

	/* image2: normal image start */
	.ram_image2.entry :
	{
		__ram_image2_text_start__ = .;
		__image2_entry_func__ = .;
		KEEP(*(SORT(.image2.entry.data*)))

		__image2_validate_code__ = .;
		KEEP(*(.image2.validate.rodata*)) 

	} > BD_RAM_NS

	.ram_image2.text :
	{
		__ram_text_start__ = .;
		*(.image2.ram.text*)
		*(.image2.net.ram.text*)
		__ram_text_end__ = .;
		. = ALIGN(16);
	} > BD_RAM_NS

	.ram_image2.data : 
	{
		__data_start__ = .;
		*(.data*)
		__data_end__ = .;
		__ram_image2_text_end__ = .;
		. = ALIGN(16);	
	} > BD_RAM_NS
  
	.ram_image2.bss :
	{
		__bss_start__ = .;
		*(.bss*)
		*(COMMON)
		__bss_end__ = .;
	} > BD_RAM_NS

	.ram_image2.nocache.data :
	{
		. = ALIGN (32);
		__ram_nocache_start__ = .;
		*(.bdsram.data*)
		. = ALIGN (32);
		__ram_nocache_end__ = .;
	} > BD_RAM_NS

	.ram_heap.data : 
	{
		. = ALIGN (32);
		*(.bfsram.data*)
		end = .;
		*(.heap.stdlib*)
		. = . + 4096;
	
	} > BD_RAM_NS

	.xip_image2.text : 
	{
		__flash_text_start__ = .;

		*(.img2_custom_signature*)

		*(.text*)
	/*	*(.image2.net.ram.text*) */		
		*(.rodata*)

		/* Add This for C++ support */
		. = ALIGN(4);
		__preinit_array_start = .;
		KEEP(*(.preinit_array))
		__preinit_array_end = .;
		. = ALIGN(4);
		__init_array_start = .;
		KEEP(*(SORT(.init_array.*)))
		KEEP(*(.init_array))
		__init_array_end = .;
		. = ALIGN(4);
		__fini_array_start = .;
		KEEP(*(SORT(.fini_array.*)))
		KEEP(*(.fini_array))
		__fini_array_end = .;	
		/*-----------------*/		

		. = ALIGN (4);
		__cmd_table_start__ = .;
		KEEP(*(.cmd.table.data*))
		__cmd_table_end__ = .;

		__flash_text_end__ = .;
	
		. = ALIGN (16);
	} > KM4_IMG2

	/* Add This for C++ support */
	.ARM.extab  : 
	{
		*(.ARM.extab* .gnu.linkonce.armextab.*) 

	} > KM4_IMG2

	.ARM.exidx :
	{
		__exidx_start = .;
		*(.ARM.exidx* .gnu.linkonce.armexidx.*)
		__exidx_end = .;
/*		end = .;*/
	} > KM4_IMG2

	__wrap_printf = 0x1010a3f5;
	__wrap_sprintf = 0x1010a471;
	__wrap_strcat = 0x10111635;
	__wrap_strchr = 0x10111745;
	__wrap_strcmp = 0x10111745;
	__wrap_strncmp = 0x101118f9;
	__wrap_strlen = 0x10111839;
	__wrap_strnlen = 0x10111a05;
	__wrap_strncat = 0x1011189d;
	__wrap_strpbrk = 0x10111a39;
	__wrap_strstr = 0x10111d25;
	__wrap_strtok = 0x1011201d;
	__wrap_strsep = 0x10111a65;
	__wrap_strtoll = 0x10111ffd;
	__wrap_strtoul = 0x101122e9;
	__wrap_strtoull = 0x10111f3d;
	__wrap_atoi = 0x101115e1;
	__wrap_strcpy = 0x101117b9;
	__wrap_strncpy = 0x1011199d;
	__wrap_memset = 0x10110ea1;
	__wrap_memcpy = 0x10110d2d;
	__wrap_memcmp = 0x10110cc9;
	__wrap_memmove = 0x10110dd9;
	__wrap_snprintf = 0x1010a49d;
	__wrap_malloc = pvPortMalloc;
	__wrap_realloc = pvPortReAlloc;
	__wrap_free = vPortFree;

	/*-----------------*/

	.bluetooth_trace.text :
	{
		__btrace_start__ = .;
		*(.BTTRACE)
		__btrace_end__ = .;
	} > BTRACE

	/* PSRAM_NS image start */
	.psram_image2.text :
	{
		__psram_image2_text_start__ = .;
		*(.psram.text*)
		__psram_image2_text_end__ = .;
	} > PSRAM_NS

	.psram_image2.data : 
	{
		. = ALIGN (32);
		*(.psram.data*)
		. = ALIGN (32);
		*(.psram.rodata*)
	} > PSRAM_NS
  
	.psram_image2.bss :
	{
		. = ALIGN (32);
		__psram_bss_start__ = .;
		*(.psram.bss*)
		__psram_bss_end__ = .;
	} > PSRAM_NS

	.psram_heap.data : 
	{
		. = ALIGN (32);
		*(.psram.heap*)
	} > PSRAM_NS
}
