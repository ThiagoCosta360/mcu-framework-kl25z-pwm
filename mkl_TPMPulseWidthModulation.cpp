#include <stdint.h>
#include <MKL25Z4.h>
#include "mkl_TPMPulseWidthModulation.h"

/*!
 *   @fn         mkl_TPMPulseWidthModulation.
 *
 *   @brief      Construtor padr�o da classe mkl_TPMPulseWidthModulation.
 *
 *   Este m�todo � o construtor padr�o da classe, que realiza a associa��o
 *   f�sica do perif�rico ao objeto de software, inicializa o perif�rico e os
 *   pinos associados.
 *
 *   @param[in]  pin - pino de sa�da do sinal PWM gerado.
 *
 */
mkl_TPMPulseWidthModulation::mkl_TPMPulseWidthModulation(tpm_Pin pin)
{
  uint8_t muxAltMask;
  uint8_t pinNumber;
  uint8_t GPIONumber;
  uint8_t chnNumber;
  uint8_t TPMNumber;
  uint8_t *baseAddress;

  setTPMParameters(pin, pinNumber, GPIONumber, chnNumber,
                   TPMNumber, muxAltMask);
  setBaseAddress(TPMNumber, &baseAddress);
  bindPeripheral(baseAddress);
  bindChannel(baseAddress, chnNumber);
  bindPin(GPIONumber, pinNumber);
  enablePeripheralClock(TPMNumber);
  enableGPIOClock(GPIONumber);
  selectMuxAlternative(muxAltMask);
}

/*!
 *   @fn         setFrequency.
 *
 *   @brief      Ajusta a frequ�ncia de opera��o do sinal PWM desejado.
 *
 *   Ajusta a frequ�ncia de opera��o do sinal PWM considerando a seguinte
 *   express�o:
 *
 *   generated_frequency = 20.94*10^6/((MODRegister + 1)*2^(divBase));
 *
 *   @param[in]  divBase - fator de divis�o.
 *               MODRegister - valor do registrador TPMxMOD.
 *
 *   @remarks    Sigla e pagina do Manual de Referencia KL25:
 *               - TPMxMOD: Status Control Register. P�g.554.
 */
void mkl_TPMPulseWidthModulation::setFrequency(tpm_Div divBase,
                                               uint16_t MODRegister)
{
  /*!
   * Desabilita opera��o (recomenda��o do fabricante).
   */
  disableOperation();
  /*!
   * Ajusta a frequ�ncia do divisor Prescale.
   */
  *addressTPMxSC = divBase;
  /*!
   * Ajusta o registrador Modulo.
   */
  *addressTPMxMOD = MODRegister;
}

/*!
 *   @fn         setDutyCycle.
 *
 *   @brief      Ajusta o duty cycle do PWM.
 *
 *   Este m�todo ajusta o duty cycle considerando a seguinte express�o:
 *   duty_cycle = 100 - (CnVRegister/(MODRegister + 1))*100, onde:
 *   CnVRegister: valor passado por par�metro para este m�todo.
 *   MODRegister: valor passado por par�metro no m�todo setFrequency.
 *
 *   @param[in]  CnVRegister - valor desejado para o registrador TPMxCnV.
 *
 *   @remarks    Sigla e pagina do Manual de Referencia KL25:
 *               - TPMxCnV: Channel Value Register. P�g. 557.
 */
void mkl_TPMPulseWidthModulation::setDutyCycle(uint16_t CnVRegister)
{
  /*!
   * Desabilita medi��o (recomenda��o do fabricante).
   */
  disableOperation();
  /*!
   * Ajusta o duty cycle.
   */
  *addressTPMxCnV = CnVRegister;
}

/*!
 *   @fn         enableOperation.
 *
 *   @brief      Habilita a opera��o do PWM.
 *
 *   Este m�todo inicia a opera��o do PWM tendo como sa�da o pino
 *   fornecido no construtor.
 *
 *   @remarks    Sigla e pagina do Manual de Referencia KL25:
 *               - TPMxSC: Status Control Register. P�g.552.
 */
void mkl_TPMPulseWidthModulation::enableOperation()
{
  /*!
   * Ajusta o m�dulo TPM para operar no modo PWM.
   */
  setPWMOperation();
  /*!
   * Habilita a opera��o escrevendo "10" no campo CMOD (Clock Mode Selection)
   * do registrador SC.
   */
  *addressTPMxSC |= 0x08;
}

/*!
 *   @fn         disableOperation.
 *
 *   @brief      Desabilita a opera��o corrente do PWM.
 *
 *   @remarks    Sigla e pagina do Manual de Referencia KL25:
 *               - TPMxSC: Status Control Register. P�g.552.
 */
void mkl_TPMPulseWidthModulation::disableOperation()
{
  *addressTPMxSC &= ~0x08;
}

/*!
 *   @fn         setPWMOperation.
 *
 *   @brief      Ajusta o m�dulo TPM para operar no modo PWM.
 *
 *   Este m�todo realiza a configura��o dos registrador de controle de
 *   status do canal para a realiza��o da opera��o de PWM.
 *
 *   @remarks    Sigla e pagina do Manual de Referencia KL25:
 *               - TPMxCnSC: Channel Status and Control Register. P�g.555.
 */
void mkl_TPMPulseWidthModulation::setPWMOperation()
{
  *addressTPMxCnSC = 0x20 | 0x08;
}
