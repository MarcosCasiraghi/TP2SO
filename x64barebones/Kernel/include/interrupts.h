 /*
 *   interrupts.h
 *
 *  Created on: Apr 18, 2010
 *      Author: anizzomc
 */

#ifndef INTERRUPTS_H_
#define INTERRUPTS_H_

#include <idtLoader.h>

void _timerHandler(void);
void _keyboardHandler(void);
void _writeHandler(void);
void _readHandler(void);
void _clearHandler(void);
void _exitHandler(void);
void _schedulerHandler(void);
void _registersHandler(void);
void _getRTCHandler(void);

void _div0Handler(void);
void _invalidOpcodeHandler(void);

void _cli(void);

void _sti(void);

void _hlt(void);

void picMasterMask(uint8_t mask);

void picSlaveMask(uint8_t mask);

//Termina la ejecución de la cpu.
void haltcpu(void);

#endif /* INTERRUPTS_H_ */
