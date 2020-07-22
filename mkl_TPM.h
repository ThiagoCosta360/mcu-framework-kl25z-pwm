#ifndef MKL_TPM_H_
#define MKL_TPM_H_

#include <MKL25Z4.h>
#include <stdint.h>

/*!
 * Enum associado � mascara do GPIO, canal, TPM e alternativa do mux PCR.
 */
typedef enum
{
  tpm_GPIOA = 0,
  tpm_GPIOB = 1 << 5,
  tpm_GPIOC = 2 << 5,
  tpm_GPIOD = 3 << 5,
  tpm_GPIOE = 4 << 5
} tpm_GPIOMask;

typedef enum
{
  tpm_CH0 = 0,
  tpm_CH1 = 1 << 8,
  tpm_CH2 = 2 << 8,
  tpm_CH3 = 3 << 8,
  tpm_CH4 = 4 << 8,
  tpm_CH5 = 5 << 8
} tpm_ChnMask;

typedef enum
{
  tpm_TPM0 = 0,
  tpm_TPM1 = 1 << 11,
  tpm_TPM2 = 2 << 11
} tpm_TPMNumberMask;

typedef enum
{
  tpm_muxAlt3 = 3 << 13,
  tpm_muxAlt4 = 4 << 13
} tpm_muxAltMask;

/*!
 * Enum associado ao fator de divis�o do divisor de frequ�ncia.
 */
typedef enum
{
  tpm_div1 = 0,
  tpm_div2,
  tpm_div4,
  tpm_div8,
  tpm_div16,
  tpm_div32,
  tpm_div64,
  tpm_div128
} tpm_Div;

/*!
 * Enum associado � borda de transi��o de detec��o.
 */
typedef enum
{
  tpm_falling = 1,
  tpm_rising,
  tpm_both
} tpm_Edge;

/*!
 * Enum associado ao perif�rico TPM e seus pinos correspondentes.
 */
typedef enum
{
  tpm_PTA0 = 0 | tpm_GPIOA | tpm_CH5 | tpm_TPM0 | tpm_muxAlt3,
  tpm_PTA4 = 4 | tpm_GPIOA | tpm_CH1 | tpm_TPM0 | tpm_muxAlt3,
  tpm_PTA5 = 5 | tpm_GPIOA | tpm_CH2 | tpm_TPM0 | tpm_muxAlt3,
  tpm_PTC1 = 1 | tpm_GPIOC | tpm_CH0 | tpm_TPM0 | tpm_muxAlt4,
  tpm_PTC2 = 2 | tpm_GPIOC | tpm_CH1 | tpm_TPM0 | tpm_muxAlt4,
  tpm_PTC3 = 3 | tpm_GPIOC | tpm_CH2 | tpm_TPM0 | tpm_muxAlt4,
  tpm_PTC4 = 4 | tpm_GPIOC | tpm_CH3 | tpm_TPM0 | tpm_muxAlt4,
  tpm_PTC8 = 8 | tpm_GPIOC | tpm_CH4 | tpm_TPM0 | tpm_muxAlt3,
  tpm_PTC9 = 9 | tpm_GPIOC | tpm_CH5 | tpm_TPM0 | tpm_muxAlt3,
  tpm_PTD0 = 0 | tpm_GPIOD | tpm_CH0 | tpm_TPM0 | tpm_muxAlt4,
  tpm_PTD1 = 1 | tpm_GPIOD | tpm_CH1 | tpm_TPM0 | tpm_muxAlt4,
  tpm_PTD2 = 2 | tpm_GPIOD | tpm_CH2 | tpm_TPM0 | tpm_muxAlt4,
  tpm_PTD3 = 3 | tpm_GPIOD | tpm_CH3 | tpm_TPM0 | tpm_muxAlt4,
  tpm_PTD4 = 4 | tpm_GPIOD | tpm_CH4 | tpm_TPM0 | tpm_muxAlt4,
  tpm_PTD5 = 5 | tpm_GPIOD | tpm_CH5 | tpm_TPM0 | tpm_muxAlt4,
  tpm_PTE29 = 29 | tpm_GPIOE | tpm_CH2 | tpm_TPM0 | tpm_muxAlt3,
  tpm_PTE30 = 30 | tpm_GPIOE | tpm_CH3 | tpm_TPM0 | tpm_muxAlt3,
  tpm_PTA12 = 12 | tpm_GPIOA | tpm_CH0 | tpm_TPM1 | tpm_muxAlt3,
  tpm_PTA13 = 13 | tpm_GPIOA | tpm_CH1 | tpm_TPM1 | tpm_muxAlt3,
  tpm_PTB0 = 0 | tpm_GPIOB | tpm_CH0 | tpm_TPM1 | tpm_muxAlt3,
  tpm_PTB1 = 1 | tpm_GPIOB | tpm_CH1 | tpm_TPM1 | tpm_muxAlt3,
  tpm_PTE20 = 20 | tpm_GPIOE | tpm_CH0 | tpm_TPM1 | tpm_muxAlt3,
  tpm_PTE21 = 21 | tpm_GPIOE | tpm_CH1 | tpm_TPM1 | tpm_muxAlt3,
  tpm_PTA1 = 1 | tpm_GPIOA | tpm_CH0 | tpm_TPM2 | tpm_muxAlt3,
  tpm_PTA2 = 2 | tpm_GPIOA | tpm_CH1 | tpm_TPM2 | tpm_muxAlt3,
  tpm_PTE22 = 22 | tpm_GPIOE | tpm_CH0 | tpm_TPM2 | tpm_muxAlt3,
  tpm_PTE23 = 23 | tpm_GPIOE | tpm_CH1 | tpm_TPM2 | tpm_muxAlt3
} tpm_Pin;

/*!
 *  @class    mkl_TPM.
 *
 *  @brief    A classe m�e "mkl_TPM" representa o perif�rico TPM.
 *
 *  @details  Esta classe � utilizada como classe m�e para os perif�ricos que
 *            est�o associados ao TPM, como o mkl_TPMDelay, mkl_TPMMeasure,
 *            mkl_TPMEventCounter, mkl_TPMPWM.
 */
class mkl_TPM
{
protected:
  /*!
   * Endere�os dos registradores associados ao perif�rico TPM e seus canais.
   */
  volatile uint32_t *addressTPMxSC;
  volatile uint32_t *addressTPMxMOD;
  volatile uint32_t *addressTPMxCNT;
  volatile uint32_t *addressTPMxCnV;
  volatile uint32_t *addressTPMxCnSC;
  volatile uint32_t *addressPortxPCRn;

  /*!
   * M�todos de bind do perif�rico, dos seus canais e do pino escolhido.
   */
  void bindPeripheral(uint8_t *baseAddress);
  void bindChannel(uint8_t *baseAddress, uint8_t chnNumber);
  void bindPin(uint8_t GPIONumber, uint8_t TPMNumber);

  /*!
   * M�todos de habilita��o de clock do perif�rico e da porta.
   */
  void enablePeripheralClock(uint8_t TPMNumber);
  void enableGPIOClock(uint8_t GPIONumber);

  /*!
   * M�todo de sele��o do mux do pino.
   */
  void selectMuxAlternative(uint8_t muxAlt);

  /*!
   * M�todos de ajuste de par�metros e endere�o base.
   */
  void setTPMParameters(tpm_Pin pin, uint8_t &pinNumber,
                        uint8_t &GPIONumber, uint8_t &chnNumber,
                        uint8_t &TPMNumber, uint8_t &muxAltMask);

  void setBaseAddress(uint8_t TPMNumber, uint8_t **baseAddress);
};

#endif //  MKL_TPM_H_
