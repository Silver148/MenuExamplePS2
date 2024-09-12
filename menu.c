#include <kernel.h>
#include <debug.h>
#include <libpad.h>
#include <sifrpc.h>
#include <loadfile.h>
#include "pad.h"
#include <unistd.h>
#include <iopheap.h>
#include <iopcontrol.h>

int opciones;

void ResetIOP()
{
	SifInitRpc(0);
	SifIopReset("", 0); 
	while(!SifIopSync()){}
	SifInitRpc(0);
	SifLoadFileInit();
	fioInit();
}

void gotoOSDSYS()
{
	ResetIOP();
	LoadExecPS2("rom0:OSDSYS", 0, NULL);
}


int main()
{
    int opcion;
    static int padBuf[256] __attribute__((aligned(64)));
    unsigned int old_pad = 0;
    unsigned int new_pad, paddata;
    int ret=0, port=0, slot=0;
    struct padButtonStatus buttons;
    int i;

    opcion = 1;

    init_scr();
    SifInitRpc(0);

    loadModules();

    padInit(0);

    ret = padPortOpen(slot, port, padBuf);

    if(ret < 0)
    {
        scr_printf("Error al iniciar controlador :(\n");
        SleepThread();
    }

    initializePad(slot, port);

    sleep(1);

    scr_clear();

    sleep(2);

    scr_printf("Ejemplo de menu para PS2\n");

    sleep(2);

    scr_printf("Creado por silverhacker\n");

    sleep(1);

    scr_setXY(5,5);

    scr_printf("1 reiniciar\n");

    scr_setXY(5,8);

    scr_printf("2 ir al menu de las memory cards\n");

    scr_setXY(5, 10);

    scr_printf("con las flechas del control puedes seleccionar una opcion(por defecto esta seleccionada la opcion 1)");

    for (;;)
    {
        i=0;
        ret=padGetState(port, slot);

        while((ret != PAD_STATE_STABLE) && (ret != PAD_STATE_FINDCTP1)) {
             if(ret==PAD_STATE_DISCONN) {
                scr_printf("El control(%d, %d) esta desconectado\n", port, slot);
            }
            ret=padGetState(port, slot);
        }
        ret = padRead(port, slot, &buttons);

        if(ret != 0)
        {
            paddata = 0xffff ^ buttons.btns; 
            new_pad = paddata & ~old_pad;
            old_pad = paddata;

            if(new_pad & PAD_UP)
            {
                opcion -= 1;

                scr_clear();

                scr_printf("La opcion 1 esta seleccionada\n");
            }

            if(new_pad & PAD_DOWN)
            {
                opcion += 1;

                scr_clear();

                scr_printf("La opcion 2 esta seleccionada\n");
            }

            if(new_pad & PAD_CROSS)
            {
                if(opcion == 1)
                {
                    ResetIOP();
                    gotoOSDSYS();
                    return 0;
                }

                if(opcion == 2)
                {
                    return 0;
                }
            }

            if(opcion == 0)
            {
                opcion += 1;
            }

            if(opcion == 3)
            {
                opcion -= 1;
            }
        }
    }

    SleepThread();

    return 0;

}
