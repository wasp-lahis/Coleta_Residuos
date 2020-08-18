# Heltech ESP32 Firmwares

Esta pasta contém os firmwares desenvolvidos para o prótotipo de **Medidor de Nível de Resíduos Sólidos**.

Placa controladora utilizada:

```
Heltech Wifi LoRa 32
```


# Configuração de ambiente (Mint/Ubuntu)

Para compilar e gravar os programas contidos nestas pastas, é necessário seguir os passos de configuração:

- Instalar o Arduino IDE: https://www.arduino.cc/en/main/software

- Adicione a placa Heltech na lista de placas do Arduino IDE: https://www.fernandok.com/2018/09/instalando-esp32-no-arduino-ide-metodo.html

- Instale as bibliotecas de Lora e de interface com o display: https://www.newtoncbraga.com.br/index.php/microcontrolador/143-tecnologia/16326-moduloesp32-heltech-mec218

- Compile e grave o programa teste *.ino* dentro da pasta ***\hello_test_heltech_lora***

- ***No caso de erros***:

  - [**path bug**] Alterar o link simbólico do python:
  ```
  sudo ln -sf /usr/bin/python3.8 /usr/bin/python
  ```

  - [**serial bug**] Instalar pyserial: https://pyserial.readthedocs.io/en/latest/pyserial.html

  - [**serial port permission bug**]
  ```
  sudo chmod a+rw /dev/ttyUSB0
  ```

# Referências

- https://www.newtoncbraga.com.br/index.php/microcontrolador/143-tecnologia/16326-moduloesp32-heltech-mec218
- https://www.fernandok.com/2018/09/instalando-esp32-no-arduino-ide-metodo.html
- https://sudomod.com/forum/viewtopic.php?t=6075
