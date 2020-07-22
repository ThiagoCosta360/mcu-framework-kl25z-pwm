#include "mkl_TPMPulseWidthModulation.h"

/*!
 * Objeto de gera��o de sinal PWM no pino PTD1.
 */
mkl_TPMPulseWidthModulation pwm(tpm_PTD1);

/*!
 *   @fn         delayMs.
 *
 *   @brief      Implementa delay de c�digo de cycle_number milissegundos.
 *
 *   @param[in]  time - tempo em milissegundos de delay desejado.
 *
 */
void delayMs(int time)
{
  int i;
  int j;

  for (i = 0; i < time; i++)
  {
    for (j = 0; j < 7000; j++)
    {
    }
  }
}

/*!
 *   @fn       main.
 *
 *   @brief    Implementa o programa exemplo do TPM no modo PWM.
 *
 *   Este programa realiza o teste da classe do perif�rico TPM no modo
 *   PWM, gerando um novo duty cycle a cada 300 ms, fazendo a cor do led
 *   azul da placa aumentar at� atingir o seu valor m�ximo e, em seguida,
 *   diminuir a sua intensidade e reiniciar o ciclo.
 *
 *   @return  Sempre retorna 0.
 */
int main(void)
{
  int duty = 500;

  while (true)
  {
    pwm.setFrequency(tpm_div16, 999);
    pwm.setDutyCycle(duty);
    pwm.enableOperation();
    delayMs(300);
    if (duty == 0)
    {
      duty = 750;
    }
    else
    {
      duty -= 250;
    }
  }

  return 0;
}
