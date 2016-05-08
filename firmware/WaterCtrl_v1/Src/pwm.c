/** @file pwm.c
 *  @brief Control the PWM out put on channel 2
 * 
 * Uses TIM2 to generate moderate pwm output based on AHP frequency.
 *
 *  @todo:
 * 	Make input pin configurable. We need to measure 8 sensor inputs
 * 	Find algorithm which less stress the output
 * 	- lower measurement period time
 * 	- combine capture inputs to gain more precision/range
 *
 * 	Multiplex inputs
 * 	- GPIO for input selection
 * 	- disable capture while inputs switch
 *
 * 	@attention:
 * 	Concurrent access to global frequency by interrupt and output loop!
 *
 * 	Questions for implementation:
 * 	1. which frequency range is expected -> prescaler
 * 	2. How to implement self stopping measure interval?
 * 		a. How to stop the measurement between channels
 *
 *  @author Dropedout
 */

/** @addtogroup MoistureMeasure
  * @{
  */



/**
  * @}
  */
