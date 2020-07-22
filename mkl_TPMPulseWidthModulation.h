#ifndef MKL_TPMPulseWidthModulation_H
#define MKL_TPMPulseWidthModulation_H

#include <stdint.h>
#include <MKL25Z4.h>
#include "mkl_TPM.h"

/*!
 *  @class    mkl_TPMPulseWidthModulation.
 *
 *  @brief    A classe implementa o servi�o PWM do perif�rico TPM.
 *
 *  @details  Esta classe implementa o servi�o PWM utilizando os
 *            perif�ricos TPM0, TPM1 ou TPM2 e os pinos correspondentes e
 *            herdando da classe m�e "mkl_TPM".
 *
 *  @section  EXAMPLES USAGE
 *
 *            Uso dos m�todos para gera��o de sinal PWM.
 *
 *              +fn setFrequency(tpm_div16, 999);
 *              +fn setDutyCycle(750);
 *              +fn enableOperation();
 */
class mkl_TPMPulseWidthModulation : public mkl_TPM
{
public:
  /*!
   * M�todo construtor padr�o da classe.
   */
  explicit mkl_TPMPulseWidthModulation(tpm_Pin pin = tpm_PTC1);

  /*!
   * M�todos de configura��o de frequ�ncia e duty cycle do sinal.
   */
  void setFrequency(tpm_Div divBase, uint16_t MODRegister);
  void setDutyCycle(uint16_t CnVRegister);

  /*!
   * M�todo de habilitar a opera��o PWM.
   */
  void enableOperation();

  /*!
   * M�todo de desabilitar a opera��o PWM.
   */
  void disableOperation();

private:
  /*!
   * M�todo de sele��o do modo de opera��o PWM.
   */
  void setPWMOperation();
};

#endif //  MKL_TPMPulseWidthModulation_H_
