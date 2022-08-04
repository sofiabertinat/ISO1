	/*
		.syntax unified

		Two slightly different syntaxes are support for ARM and THUMB instructions.
		The default, divided, uses the old style where ARM and THUMB instructions had their own,
		separate syntaxes. The new, unified syntax, which can be selected via the
		.syntax directive
	*/

	.syntax unified
	.global PendSV_Handler



	/*
		Se cambia a la seccion .text, donde se almacena el programa en flash
	*/
	.text

	/*
		Indicamos que la proxima funcion debe ser tratada como codigo thumb al ser compilada
		Ver documentacion al respecto para mas detalles
	*/
	.thumb_func



PendSV_Handler:

	/* Disable interruptions */
	cpsid i

	/* FPU siempre activa */
	/* Compruebo si el bit 4 del EXEC_RETURN es cero, este esta almacenado en LR*/
	tst lr,0x10
	it eq
	Vpusheq {s16-s31}	/* PUSH de registros FPU */

	/* Guardar los registros R4-R11 y el valor de LR, que en este punto es EXEC_RETURN
	* El push se hace al reves de como se escribe en la instruccion, por lo que LR
	* se guarda en la posicion 9 (luego del stack frame). */
	push {r4-r11,lr}
	/* El pasaje de argumentos a getcontextSwitch se hace como especifica el AAPCS siendo
	* el unico argumento pasado por RO, y el valor de retorno tambien se almacena en R0 */
	mrs r0,msp
	/* Como la funcion getcontextSwitch se llama con un branch con link, el valor del LR
	* es modificado guardando la direccion de retorno una vez se complete la ejecucion de la funcion */
	bl getcontextSwitch
	msr msp,r0
	/* Recupero los valores de registros */
	pop {r4-r11,lr}

	/* Compruebo si el bit 4 del EXEC_RETURN es cero*/
	tst lr,0x10
	it eq
	Vpopeq {s16-s31}	/* POP de registros FPU */

	/* Enable interruptions */
	cpsie i

	/* Se hace un branch indirect con el valor de LR que es nuevamente EXEC_RETURN */
	bx lr


	/*
	* NOTA: El primer ingreso a este handler (luego del reset) implica que el push se hace sobre el
	* stack inicial, ese stack se pierde porque no hay seguimiento del MSP en el primer ingreso
	*/

