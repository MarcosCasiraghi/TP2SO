#include <stdint.h>
#include <idtLoader.h>
#include <defs.h>
#include <interrupts.h>

#pragma pack(push)		/* Push de la alineación actual */
#pragma pack (1) 		/* Alinear las siguiente estructuras a 1 byte */

/* Descriptor de interrupcion */
typedef struct {
  uint16_t offset_l, selector;
  uint8_t cero, access;
  uint16_t offset_m;
  uint32_t offset_h, other_cero;
} DESCR_INT;

#pragma pack(pop)		/* Reestablece la alinceación actual */

DESCR_INT * idt = (DESCR_INT *) 0;	// IDT de 255 entradas

static void setup_IDT_entry (int index, uint64_t offset);

void load_idt() {

  //timer tick
  setup_IDT_entry (0x20, (uint64_t)&_timerHandler);
  //keyboard
  setup_IDT_entry(0x21, (uint64_t)&_keyboardHandler);
  //write
  setup_IDT_entry(0x80, (uint64_t)&_writeHandler);
  //read
  setup_IDT_entry (0x81, (uint64_t)&_readHandler);
  //clear screen
  setup_IDT_entry(0x82, (uint64_t)&_clearHandler);

  //exit for programs
  setup_IDT_entry(0x83, (uint64_t)&_exitHandler);
  //add task to scheduler
  setup_IDT_entry(0x84, (uint64_t)&_schedulerHandler);
  //fill recieved buffers with registers
  setup_IDT_entry(0x85, (uint64_t)&_registersHandler);
  //fill recieved buffer with present time and date
  setup_IDT_entry(0x86, (uint64_t)&_getRTCHandler);
  //fill recieved buffer with memory dump
  setup_IDT_entry(0x87, (uint64_t)&_getMemHandler);

  setup_IDT_entry(0x88,(uint64_t)&_exec);

  setup_IDT_entry(0x89, (uint64_t)&_mallocHandler);

  setup_IDT_entry(0x90, (uint64_t)&_freeHandler);

  setup_IDT_entry(0x91, (uint64_t)&_mmStatusHandler);


  setup_IDT_entry(0x92, (uint64_t)&_psHandler);

  setup_IDT_entry(0x93, (uint64_t)&_killHandler);

  setup_IDT_entry(0x94, (uint64_t)&_niceHandler);

  setup_IDT_entry(0x95, (uint64_t)&_blockHandler);

  setup_IDT_entry(0x96, (uint64_t)&_getPIDHandler);

  setup_IDT_entry(0x97, (uint64_t)&_semOpenHandler);

  setup_IDT_entry(0x98, (uint64_t)&_semCloseHandler);

  setup_IDT_entry(0x99, (uint64_t)&_semPostHandler);

  setup_IDT_entry(0x8A, (uint64_t)&_semWaitHandler);

  setup_IDT_entry(0x8B, (uint64_t)&_semStatusHandler);

  setup_IDT_entry(0x8C, (uint64_t)&_openPipeHandler);

  setup_IDT_entry(0x8D, (uint64_t)&_closePipeHandler);

  setup_IDT_entry(0x8E, (uint64_t)&_readPipeHandler);

  setup_IDT_entry(0x8F, (uint64_t)&_writePipeHandler);

  setup_IDT_entry(0x9A, (uint64_t)&_pipeStatusHandler);

    setup_IDT_entry(0x9B, (uint64_t)&_yieldHandler);

  //divide by 0 exception
  setup_IDT_entry (0x00, (uint64_t)&_div0Handler);

  //invalid opcode exception
  setup_IDT_entry(0x06, (uint64_t)&_invalidOpcodeHandler);



	//Solo interrupcion timer tick y teclado habilitadas
	picMasterMask(0xFC);
	picSlaveMask(0xFF);

	_sti();
}

static void setup_IDT_entry (int index, uint64_t offset) {
  idt[index].selector = 0x08;
  idt[index].offset_l = offset & 0xFFFF;
  idt[index].offset_m = (offset >> 16) & 0xFFFF;
  idt[index].offset_h = (offset >> 32) & 0xFFFFFFFF;
  idt[index].access = ACS_INT;
  idt[index].cero = 0;
  idt[index].other_cero = (uint64_t) 0;
}
