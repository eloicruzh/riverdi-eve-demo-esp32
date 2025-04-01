#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "platform.h"
#include "esp_log.h"
#include "App_Common.h"

Gpu_Hal_Context_t host, *phost;

static void SAMAPP_GPU_Ball_Stencil();

static void check_spi(Gpu_Hal_Context_t *phost)
{
    // Read the ID register, which should be 0x7C for BT817/818
    uint8_t id = Gpu_Hal_Rd8(phost, REG_ID);

    ESP_LOGI("APP_MAIN", "BT817 REG_ID = 0x%02X\n", id);


    if (id == 0x7C)
    {
      ESP_LOGI("APP_MAIN", "SPI OK, BT817 recognized!\n");
    }
    else
    {
      ESP_LOGI("APP_MAIN","SPI FAIL, got 0x%02X instead of 0x7C\n", id);

        // If you get 0x00, 0xFF, or anything else, 
        // it means there is a wiring or pin-configuration issue.
    }
}

static void show_black_background_and_4buttons(Gpu_Hal_Context_t *phost)
{
    // 1) Set the backlight to maximum (100%)
    //    For Riverdi EVE modules, REG_PWM_DUTY=128 typically equals 100% duty
    Gpu_Hal_Wr16(phost, REG_PWM_HZ, 4000);     // 4kHz backlight PWM
    Gpu_Hal_Wr8(phost, REG_PWM_DUTY, 128);     // 128 => 100% brightness

    // 2) Begin a new Display List
    Gpu_CoCmd_Dlstart(phost);

    // 3) Clear the screen to black
    App_WrCoCmd_Buffer(phost, CLEAR_COLOR_RGB(0, 0, 0));  // black
    App_WrCoCmd_Buffer(phost, CLEAR(1, 1, 1));

    // 4) Draw white buttons
    //    We’ll use the built-in coprocessor command BUTTON(x, y, w, h, font, options, string)
    //    Adjust x/y/w/h as you like. Font 28 is large but easy to read.

    // Set color to white
    App_WrCoCmd_Buffer(phost, COLOR_RGB(255, 255, 255));

    // Place 4 buttons in a vertical column
    Gpu_CoCmd_Button(phost, 40, 40, 120, 40, 28, 0, "BUTTON 1");
    Gpu_CoCmd_Button(phost, 40, 100, 120, 40, 28, 0, "BUTTON 2");
    Gpu_CoCmd_Button(phost, 40, 160, 120, 40, 28, 0, "BUTTON 3");
    Gpu_CoCmd_Button(phost, 40, 220, 120, 40, 28, 0, "BUTTON 4");

    // 5) End Display List and Swap
    App_WrCoCmd_Buffer(phost, DISPLAY());
    Gpu_CoCmd_Swap(phost);

    // Flush the command buffer to the BT817
    App_Flush_Co_Buffer(phost);
    // Wait until the co-processor has processed everything
    Gpu_Hal_WaitCmdfifo_empty(phost);
}

static void show_black_and_white_buttons(Gpu_Hal_Context_t *phost)
{
     // 1) Set backlight to full brightness (128 => ~100% on Riverdi EVE4 modules)
     Gpu_Hal_Wr16(phost, REG_PWM_HZ, 4000);
     Gpu_Hal_Wr8(phost, REG_PWM_DUTY, 128);
 
     // 2) Begin new display list
     Gpu_CoCmd_Dlstart(phost);
 
     // 3) Clear the screen to black
     App_WrCoCmd_Buffer(phost, CLEAR_COLOR_RGB(0, 0, 0));  // black background
     App_WrCoCmd_Buffer(phost, CLEAR(1, 1, 1));
 
     // 4) Set coprocessor colors for buttons:
     //    FGCOLOR => the main fill color of the button
     //    BGCOLOR => used for gradients if not using OPT_FLAT
     //    Here we want a white fill with no gradient. We'll do BG=white as well.
     Gpu_CoCmd_BgColor(phost, 0xFFFFFF);  // white
     Gpu_CoCmd_FgColor(phost, 0xFFFFFF);  // white
 
     // 5) The text color is set via COLOR_RGB.
     //    So we pick black text for good contrast against white.
     //    Also set alpha to fully opaque, just in case.
     App_WrCoCmd_Buffer(phost, COLOR_A(255));
     App_WrCoCmd_Buffer(phost, COLOR_RGB(0,0,0));  // black text
 
     // 6) Draw 4 flat buttons using a known built-in font, e.g. 26
     //    (x, y, w, h, font, options, "label")
     Gpu_CoCmd_Button(phost, 40,  40, 120, 40, 26, OPT_FLAT, "BUTTON 1");
     Gpu_CoCmd_Button(phost, 40, 100, 120, 40, 26, OPT_FLAT, "BUTTON 2");
     Gpu_CoCmd_Button(phost, 40, 160, 120, 40, 26, OPT_FLAT, "BUTTON 3");
     Gpu_CoCmd_Button(phost, 40, 220, 120, 40, 26, OPT_FLAT, "BUTTON 4");
 
     // 7) End display list and swap
     App_WrCoCmd_Buffer(phost, DISPLAY());
     Gpu_CoCmd_Swap(phost);
 
     App_Flush_Co_Buffer(phost);
     Gpu_Hal_WaitCmdfifo_empty(phost);
}




void app_main(void)
{
  ESP_LOGI("APP_MAIN", "Starting up ...");

  phost = &host;

  /* Init HW Hal */
  App_Common_Init(&host);

  check_spi(phost);

  show_black_and_white_buttons(&host);

  /* Screen Calibration*/
  //App_Calibrate_Screen(&host);

  /* Show Bridgetek logo */
  //App_Show_Logo(&host);

  /* Bouncing ball demo */
  //xTaskCreate(SAMAPP_GPU_Ball_Stencil, "ball-task", 4096, NULL,  tskIDLE_PRIORITY, NULL);
}


static void
SAMAPP_GPU_Ball_Stencil()
{
  uint8_t xflag, yflag;
  int16_t xball, yball, rball, pixel_precision, gridsize;
  int32_t displ, dispr, dispa, dispb;

  /* grid margins */
  displ = 10;
  dispr = (DispWidth - 10);
  dispa = 50;
  dispb = (DispHeight - 10);

  /* grid size */
  gridsize = 20;

  /* ball dimensions */
  xball = (DispWidth/2);
  yball = (DispHeight/2);
  rball = (DispWidth/8);
  xflag = 1;
  yflag = 1;

  dispr -= ((dispr - displ)%gridsize);
  dispb -= ((dispb - dispa)%gridsize);

  /* endless loop */
  while(1)
    {
      /* ball movement */
      if(((xball + rball + 2) >= dispr) || ((xball - rball - 2) <= displ))
        xflag ^= 1;

      if(((yball + rball + 8) >= dispb) || ((yball - rball - 8) <= dispa))
        yflag ^= 1;

      if(xflag)
        xball += 2;
      else
        xball -= 2;

      if(yflag)
        yball += 8;
      else
        yball -= 8;

      /* set the precision of VERTEX2F coordinates */
#if defined (IPS_70) || (IPS_101)
      /* VERTEX2F range: -2048 to 2047 */
      App_WrDl_Buffer(phost, VERTEX_FORMAT(3));
      pixel_precision = 8;
#else
      /* use default VERTEX_FORMAT(3) with VERTEX2F range: -1024 to 1023 */
      pixel_precision = 16;
#endif

      /* init and set background */
      App_WrDl_Buffer(phost, CLEAR_COLOR_RGB(255, 255, 255));
      App_WrDl_Buffer(phost, CLEAR(1, 1, 1));
      App_WrDl_Buffer(phost, STENCIL_OP(INCR,INCR));
      App_WrDl_Buffer(phost, COLOR_RGB(0, 0, 0));

      /* draw grid */
      App_WrDl_Buffer(phost, LINE_WIDTH(pixel_precision));
      App_WrDl_Buffer(phost, BEGIN(LINES));

      for(uint16_t i=0; i<=((dispr - displ)/gridsize); i++)
        {
          App_WrDl_Buffer(phost, VERTEX2F((displ + i*gridsize)* \
                          pixel_precision,dispa*pixel_precision));
          App_WrDl_Buffer(phost, VERTEX2F((displ + i*gridsize)* \
                          pixel_precision,dispb*pixel_precision));
        }

      for(uint16_t i=0; i<=((dispb - dispa)/gridsize); i++)
        {
          App_WrDl_Buffer(phost, VERTEX2F(displ*pixel_precision,
                          (dispa + i*gridsize)*pixel_precision));
          App_WrDl_Buffer(phost, VERTEX2F(dispr*pixel_precision,
                          (dispa + i*gridsize)*pixel_precision));
        }
      App_WrDl_Buffer(phost, END());

      /* add simple text using built-in fonts */
      {
        Gpu_Fonts_t   font;
        uint8_t       font_size;
        uint32_t      font_table;
        uint32_t      text_hoffset, text_voffset;

#if defined (NTP_35) || (RTP_35) || (CTP_35) || (IPS_35) || \
            (NTP_43) || (RTP_43) || (CTP_43) || (IPS_43)
        const uint8_t text[] = "Riverdi EVE Demo";
#elif defined (NTP_50) || (RTP_50) || (CTP_50) || (IPS_50) || \
              (NTP_70) || (RTP_70) || (CTP_70) || (IPS_70)
        const uint8_t text[] = "Riverdi EVE Demo - https://www.riverdi.com";
#elif defined (IPS_101)
        const uint8_t text[] = "Riverdi EVE Demo - https://www.riverdi.com - " \
                               "contact@riverdi.com";
#endif

        text_hoffset = displ; /* set the same offset like for grid */
        text_voffset = 5;

        font_size = 30;
        font_table = Gpu_Hal_Rd32(phost, ROMFONT_TABLEADDRESS);

        Gpu_Hal_RdMem(phost, (font_table + (font_size-16)*GPU_FONT_TABLE_SIZE),
                      (uint8_t*)&font, GPU_FONT_TABLE_SIZE);

        App_WrDl_Buffer(phost, COLOR_RGB(0, 96, 169));
        App_WrDl_Buffer(phost, BEGIN(BITMAPS));
        App_WrDl_Buffer(phost, BITMAP_HANDLE((font_size%32)));

        for (uint8_t cnt = 0; cnt < sizeof(text)-1; cnt++)
          {
            App_WrDl_Buffer(phost, CELL(text[cnt]));
            App_WrDl_Buffer(phost, VERTEX2F(text_hoffset*pixel_precision,
                            text_voffset*pixel_precision));
            text_hoffset += font.FontWidth[text[cnt]];
          }
        App_WrDl_Buffer(phost, END());
      }

      /* draw ball and shadow */
      App_WrDl_Buffer(phost, COLOR_MASK(1,1,1,1));
      App_WrDl_Buffer(phost, STENCIL_FUNC(ALWAYS,1,255));
      App_WrDl_Buffer(phost, STENCIL_OP(KEEP,KEEP));
      App_WrDl_Buffer(phost, COLOR_RGB(255, 255, 255));
      App_WrDl_Buffer(phost, POINT_SIZE(rball*16));
      App_WrDl_Buffer(phost, BEGIN(FTPOINTS));
      App_WrDl_Buffer(phost, VERTEX2F((xball - 1)*pixel_precision,
                                      (yball - 1)*pixel_precision));
      App_WrDl_Buffer(phost, COLOR_RGB(0, 0, 0));
      App_WrDl_Buffer(phost, COLOR_A(160));
      App_WrDl_Buffer(phost, VERTEX2F((xball+pixel_precision)*pixel_precision,
                                      (yball+8)*pixel_precision));
      App_WrDl_Buffer(phost, COLOR_A(255));
      App_WrDl_Buffer(phost, COLOR_RGB(254, 172, 0));
      App_WrDl_Buffer(phost, VERTEX2F(xball*pixel_precision,
                                      yball*pixel_precision));
      App_WrDl_Buffer(phost, COLOR_RGB(255, 255, 255));
      App_WrDl_Buffer(phost, STENCIL_FUNC(GEQUAL,1,1));
      App_WrDl_Buffer(phost, STENCIL_OP(KEEP,KEEP));
      App_WrDl_Buffer(phost, VERTEX2F(xball*pixel_precision,
                                      yball*pixel_precision));
      App_WrDl_Buffer(phost, END());

      /* display */
      App_WrDl_Buffer(phost, DISPLAY());

      /* download display list into DL RAM */
      App_Flush_DL_Buffer(phost);

      /* do a swap */
      Gpu_Hal_DLSwap(phost, DLSWAP_FRAME);

    } /* while */
}
