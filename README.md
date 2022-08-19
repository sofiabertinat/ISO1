# ISO1
Implementación de un sistema operativo
#############

IMPLEMENTACIÓN OS:

  - IRQ    -> Se crean dos vectores, uno para almacenar los punteros a las isr definidas por el usuario, y otro para almacenar un valor pasado por parámetro al                      registrar la isr, que correspode a un uint8_t, este puede de ser usado por el usuario a su conveniencia. En mi aplicación lo utilice para usar la misma                función de isr para todas las teclas y poder diferenciar de quien provenia la interrupción.
             Mediante las funcion os_isr_register() se define la isr correspondiente a una interrupción, y  os_isr_unregister() permite eliminar esta.
             Todas las interrupciones llaman a la función os_IRQHandler(), la cual se encarga de ejecutar la isr definida por el usuario, limpia la irq pendiente y                  hace un rescheduler de ser necesario.
             Se crea una función (os_my_IRQ_Init) para inicializar la configuración de la IRQ a utilizar en la aplicación, en este caso los 8 canales para el manejo de              las 4 teclas.
             La definición de la isr debe de devuelve un booleano indicando si se requiere un rescheduler, y debe de tener como parámetro un uint8_t.
  
  - Colas  ->  Permite el pasaje de cualquier tipo de elemento.
              Se implementa el bloqueo de las tarea que llama a os_queue_write si la cola esta llena, con excepción de si la tarea proviene de una isr, esto debe                     indicarse en el parámetro correspondiente de la función, mediante un booleano.
              Se implementa el bloqueo de las tarea que llama a os_queue_read si la cola esta vacía.
              Se debe de tener en cuenta que el usuario debe de manejar correctamente la memoria de los datos enviados a traves de la cola. Se recomienda la                         utilizaciónde malloc para obtener un lugar de memoria para almacenar el dato a envíar, en consecuencia, el usuario debe de liberar la misma una vez                     recibido el dato por la cola.
              
   -Semaforos binarios
   
   - Delay
   
   - Manejo de errores -> Mediante las funciones: tickHook(),  errorHook(),  returnHook();
  
PARA LA APLICACIÓN CREADA:

+ Se crean 2 colas:
  - display     
  - control Tec  

+ Se crean 2 semáforos:
  - write     
  - led       
  
+ Se crean 4 tareas:
  - write      ->     escribe en la cola display el valor de un contador incrementado cada vez que se enciende el LED2.
                      El valor del LED2 cambia su valor cada 20s, esto se logra mediante la ultilización del os_delay implementado en el OS.
                      
  - dispaly    ->     espera recibir un dato por la cola display y lo escribe por la UART.
  
  - led on     ->     espera poder tomar el semáforo led y luego enciende el LED1.
  
  - led off    ->     apaga el LED1, luego de un os_delay de 1ms libera el semáforo led, y vuelve a hacer otro os_delay de 1ms antes de repetir el loop.
  
  - botton     ->     maneja la pulsación de las 4 teclas de la CIAA, mediante una estructura buttons_SM_t que maneja la maquina de estados de cada tecla. 
                      Espera la llegada de un dato a la cola control Tec, la cual recibe desde un servicio de interrupción la indicación de flanco de subida o bajada                         de una de las teclas.
                      Una vez establecida una pulsación se envía a la cola display el numero id correspondiente a la tecla pulsada (TEC1:201, TEC2:201, TEC3:203,                             TEC4:204).
