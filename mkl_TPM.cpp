#include "mkl_TPM.h"

/*!
 *   @fn         bindPeripheral
 *
 *   @brief      Associa o objeto de software ao perif�rico de hardware.
 *
 *   Este m�todo associa ao objeto de software o perif�rico de hardware,
 *   utilizando a inicializa��o dos ponteiros para os endere�os de mem�ria
 *   dos registradores correspondentes.
 *
 *   @param[in]  baseAddress - o endere�o base do perif�rico TPM.
 *
 *   @remarks    Sigla e pagina do Manual de Referencia KL25:
 *               - TPMxSC: Status Control Register. P�g. 552.
 *               - TPMxCNT: Counter Register. P�g.554.
 *               - TPMxMOD: Modulo Register. P�g. 554.
 */

void mkl_TPM::bindPeripheral(uint8_t *baseAddress)
{
  addressTPMxSC = (volatile uint32_t *)(baseAddress);
  addressTPMxCNT = (volatile uint32_t *)(baseAddress + 0x4);
  addressTPMxMOD = (volatile uint32_t *)(baseAddress + 0x8);
}

/*!
 *   @fn         bindChannel
 *
 *   @brief      Associa o canal do objeto de software ao hardware
 *
 *   Este m�todo associa os atributos do canal do objeto de software ao
 *   seu correspondente do perif�rico hardware.
 *
 *   @param[in]  baseAddress - o endere�o base do perif�rico TPM.
 *               chnNumber - o n�mero do canal do perif�rico TPM.
 *
 *   @remarks    Sigla e pagina do Manual de Referencia KL25:
 *               - TPMxCnV: Channel Value Register. P�g.557.
 *               - TPMxCnSC: Channel Status Control Register. P�g.555.
 */
void mkl_TPM::bindChannel(uint8_t *baseAddress,
                          uint8_t chnNumber)
{
  addressTPMxCnV = (volatile uint32_t *)(baseAddress + 0x10 + 8 * chnNumber);
  addressTPMxCnSC = (volatile uint32_t *)(baseAddress + 0xC + 8 * chnNumber);
}

/*!
  *   @fn         bindPin
  *
  *   @brief      Associa o pino do objeto ao perif�rico de hardare.
  *
  *   Este m�todo associa o pino do objeto de software ao pino do perif�rico.
  *
  *   @param[in]  pinNumber - o n�mero do pino do objeto de software.
  *
  *   @remarks    Sigla e pagina do Manual de Referencia KL25:
  *               - PCR: Pin Control Register. P�g.183.
  */
void mkl_TPM::bindPin(uint8_t GPIONumber, uint8_t pinNumber)
{
  addressPortxPCRn = (volatile uint32_t *)(0x40049000 + 0x1000 * GPIONumber + 4 * pinNumber);
}

/*!
 *   @fn         enablePeripheralClock
 *
 *   @brief      Habilita o clock do perif�rico de hardware.
 *
 *   Este m�todo habilita o clock do perif�rico TPM solicitado.
 *
 *   @remarks    Sigla e pagina do Manual de Referencia KL25:
 *               - SCGC6: System Control Gating Clock Register 6. P�g.207.
 *               - SOPT2: System Options Register 2. P�g.195.
 */
void mkl_TPM::enablePeripheralClock(uint8_t TPMNumber)
{
  SIM_SCGC6 |= SIM_SCGC6_TPM0_MASK << TPMNumber;
  SIM_SOPT2 |= SIM_SOPT2_TPMSRC(1);
}

/*!
 *   @fn         enableGPIOClock
 *
 *   @brief      Habilita o clock do GPIO do pino.
 *
 *   Este m�todo habilita o clock do perif�rico GPIO do pino passado por
 *   par�metro.
 *
 *   @param[in]  GPIONumber - o n�mero do GPIO correspondente ao pino.
 *
 *   @remarks    Sigla e pagina do Manual de Referencia KL25:
 *               - SCGC5: System Control Gating Clock Register 5. P�g.199.
 */
void mkl_TPM::enableGPIOClock(uint8_t GPIONumber)
{
  SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK << GPIONumber;
}

/*!
 *   @fn         selectMuxAlternative
 *
 *   @brief      Seleciona a alternativa de trabalho do pino.
 *
 *   Este m�todo seleciona o modo de opera��o do pino correspondente para o
 *   modo de opera��o TPM.
 *
 *   @param[in]  muxAlt - alternativa de opera��o do pino para o canal e TPM
 *                        especificados.
 *
 *   @remarks    Sigla e pagina do Manual de Referencia KL25:
 *               - PCR: Pin Control Register. P�g.183.
 */
void mkl_TPM::selectMuxAlternative(uint8_t muxAlt)
{
  *addressPortxPCRn = PORT_PCR_MUX(muxAlt);
}

/*!
 *   @fn         setTPMParameters
 *
 *   @brief      Ajusta os par�metros do TPM.
 *
 *   Este m�todo ajusta os par�metros do TPM (n�meros de pino, GPIO,
 *   canal, TPM) e a m�scara de sele��o do mux, de acordo com o membro
 *   da enum passado no construtor da classe filha.
 *
 *   @param[in]  pin - pino passado para o construtor da classe filha.
 *               pinNumber - n�mero do pino.
 *               GPIONumber - n�mero do GPIO.
 *               chnNumber - n�mero do canal.
 *               TPMNumber - n�mero do TPM.
 *               muxAltMask - m�scara de sele��o do mux.
 */
void mkl_TPM::setTPMParameters(tpm_Pin pin, uint8_t &pinNumber,
                               uint8_t &GPIONumber, uint8_t &chnNumber,
                               uint8_t &TPMNumber, uint8_t &muxAltMask)
{
  pinNumber = pin & 0x1F;
  GPIONumber = (pin >> 5) & 0x7;
  chnNumber = (pin >> 8) & 0x7;
  TPMNumber = (pin >> 11) & 0x3;
  muxAltMask = (pin >> 13) & 0x7;
}

/*!
 *   @fn         setBaseAddress
 *
 *   @brief      Ajusta o endere�o base do TPM.
 *
 *   Este m�todo ajusta o endere�o base do TPM a partir do n�mero do
 *   TPM.
 *
 *   @param[in]  TPMNumber - n�mero do TPM.
 *               baseAddress - ponteiro para o endere�o base do TPM.
 */
void mkl_TPM::setBaseAddress(uint8_t TPMNumber, uint8_t **baseAddress)
{
  *baseAddress = (uint8_t *)(TPM0_BASE + 0x1000 * TPMNumber);
}
