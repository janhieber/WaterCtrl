/*
 * syscalls.c
 *
 *  Created on: 24.04.2015
 *      Author: jan
 */

#include <stdio.h>
#include <sys/stat.h>
#include <stm32f0xx.h>

int _fstat (int fd, struct stat *pStat)
{
	pStat->st_mode = S_IFCHR;
	return 0;
}

int _close(int a)
{
	return -1;
}

int _write (int fd, char *pBuffer, int size)
{
	for (int i = 0; i < size; i++)
	{
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART1, pBuffer[i]);
	}
	return size;
}

int _isatty (int fd)
{
	return 1;
}

int _lseek(int a, int b , int c)
{
	return -1;
}

int _read (int fd, char *pBuffer, int size)
{
	return 0;
	for (int i = 0; i < size; i++)
	{
		//while ((USART1->SR & USART_SR_RXNE) == 0)
		//{
		//}

		//pBuffer[i] = USART_ReceiveData(USART1);
	}
	return size;
}

/*
caddr_t _sbrk(int increment)
{
	extern char end asm("end");
	register char *pStack asm("sp");

	static char *s_pHeapEnd;

	if (!s_pHeapEnd)
		s_pHeapEnd = &end;

	if (s_pHeapEnd + increment > pStack)
		return (caddr_t)-1;

	char *pOldHeapEnd = s_pHeapEnd;
	s_pHeapEnd += increment;
	return (caddr_t)pOldHeapEnd;
}
*/
