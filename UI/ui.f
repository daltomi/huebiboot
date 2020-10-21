# data file for the Fltk User Interface Designer (fluid)
version 1.0304
header_name {.hpp}
code_name {.cxx}
decl {\#include <FL/Fl_Shared_Image.H>} {public global
}

decl {\#include <FL/Fl_Image.H>} {public global
}

decl {\#include <FL/fl_ask.H>} {public global
}

decl {\#include <FL/Fl_Native_File_Chooser.H>} {public global
}

decl {\#include <FL/filename.H>} {public global
}

decl {\#include "icons/icons.h"} {
  comment {Debe estar ultimo para acceder el header Fl_Image.H} public global
}

Function {ShowWindowModal(Fl_Window * window)} {open C return_type void
} {
  code {window->set_modal ();
window->show ();
while (window->shown ())
{
  Fl::wait ();
}} {}
}

widget_class UIMain {
  label huebiboot open
  xywh {460 258 380 235} type Double color 55 selection_color 0
  code0 {fl_register_images();}
  code1 {selectFormat = FORMATS::FAT;}
  class Fl_Double_Window modal visible
} {
  Fl_Box boxTitle {
    xywh {0 40 75 195} box FLAT_BOX color 55 align 512
    code0 {o->image (get_icon_title());}
  }
  decl {enum FORMATS {FAT, NTFS, EXT3, EXT4};} {
    comment {Tipos de formatos.} public global
  }
  decl {FORMATS selectFormat;} {public local
  }
  code {static int col_widths [] = {80, 80, 0};} {}
  Function {OnChoiceEvent(Fl_Choice *choice, void *data)} {open private return_type {static void}
  } {
    code {UIMain * ui = (UIMain*)(data);

ui->selectFormat = (UIMain::FORMATS)choice->value();} {}
  }
  Fl_Group groupMain {
    xywh {0 0 416 235} hide
  } {
    Fl_Group {} {
      xywh {80 30 336 170} color 50
    } {
      Fl_Browser browserDevices {
        label Dispositivos
        callback OnBrowserEvent
        xywh {90 43 281 93} type Hold box UP_BOX color 55 selection_color 171 labelsize 11 align 5 textsize 11
        code0 {o->column_widths (col_widths);}
        code1 {o->column_char ('\\t');}
      }
      Fl_Choice choiceFormat {
        label {Tipo formato (part .\#3)}
        user_data this
        callback OnChoiceEvent
        comment {¡¡ No cambiar el orden !!
Debe coincidir con FORMATS} open
        xywh {90 166 281 20} box THIN_UP_BOX down_box THIN_UP_BOX color 50 selection_color 171 labelsize 11 align 5 textsize 11
      } {
        MenuItem {} {
          label {FAT-32}
          user_data {FORMATS::FAT}
          xywh {0 0 36 20} labelsize 11
        }
        MenuItem {} {
          label NTFS
          user_data {FORMATS::NTFS}
          xywh {0 0 36 20} labelsize 11
        }
        MenuItem {} {
          label {EXT-3}
          user_data {FORMATS::EXT3}
          xywh {0 0 36 20} labelsize 11
        }
        MenuItem {} {
          label {EXT-4}
          user_data {FORMATS::EXT4}
          xywh {0 0 36 20} labelsize 11
        }
      }
    }
    Fl_Group {} {open
      xywh {90 195 276 40}
    } {
      Fl_Button buttonFormat {
        label Comenzar
        callback OnButtonEvent
        xywh {195 195 80 25} box FLAT_BOX down_box FLAT_BOX color 7 selection_color 171 labelfont 1 labelsize 11 labelcolor 37 align 256
        code0 {o->image (get_icon_yes_activate());}
        code1 {o->deimage (get_icon_yes_deactivate());}
      }
      Fl_Button buttonCancel {
        label Cancelar
        callback OnButtonEvent
        xywh {286 195 80 25} box FLAT_BOX down_box FLAT_BOX color 7 selection_color 171 labelfont 1 labelsize 11 labelcolor 37 align 256
        code0 {o->image (get_icon_no());}
      }
    }
    Fl_Group {} {open
      xywh {0 0 380 29}
    } {
      Fl_Button buttonToolInfo {
        callback OnButtonEvent
        tooltip {Ver información...} xywh {5 5 24 24} box FLAT_BOX down_box FLAT_BOX color 7 selection_color 171 labelsize 9 align 512
        code0 {o->image (get_icon_list());}
      }
      Fl_Button buttonToolBackup {
        callback OnButtonEvent
        tooltip {Copia de seguridad...} xywh {40 5 24 24} box FLAT_BOX down_box FLAT_BOX color 7 selection_color 171 labelsize 9 align 512
        code0 {o->image (get_icon_backup());}
      }
      Fl_Button buttonToolHelp {
        callback OnButtonEvent
        tooltip {Ayuda...} xywh {346 5 24 24} box FLAT_BOX down_box FLAT_BOX color 7 selection_color 171 labelsize 9 align 512
        code0 {o->image (get_icon_help());}
      }
    }
  }
  Fl_Group groupSplash {
    xywh {80 55 300 180} color 50
  } {
    Fl_Box boxSplashWait {
      label {Por favor, conecte su dispositivo USB ...}
      xywh {80 105 290 20} box FLAT_BOX color 55 labelsize 11 align 21
    }
    Fl_Button buttonQuit {
      label Salir
      callback OnButtonEvent
      xywh {290 195 80 25} box FLAT_BOX down_box FLAT_BOX color 7 selection_color 171 labelfont 1 labelsize 11 labelcolor 37 align 256
      code0 {o->image (get_icon_no());}
    }
    Fl_Box boxSplashNotFound {
      label {No se encontró ningún dispositivo, reintentando ...}
      xywh {80 125 290 20} box FLAT_BOX color 7 labelsize 11 labelcolor 1 align 21 hide
    }
  }
  Fl_Group groupBackup {
    xywh {80 40 300 195} hide
  } {
    Fl_Box boxBackupTitle {
      label {Copia de seguridad}
      xywh {90 45 265 25} color 0 selection_color 0 labelfont 1 labelsize 11 labelcolor 171 align 276
      code0 {o->image (get_icon_save());}
    }
    Fl_Button buttonBackupCancel {
      label Cancelar
      callback OnButtonEvent
      xywh {290 195 80 25} box FLAT_BOX down_box FLAT_BOX color 7 selection_color 171 labelfont 1 labelsize 11 labelcolor 37 align 256
      code0 {o->image (get_icon_no());}
    }
    Fl_Box boxBackupFile {
      label {Archivo: }
      xywh {90 100 265 25} color 0 selection_color 0 labelsize 11 align 276
    }
    Fl_Box boxBackupBytes {
      xywh {90 125 265 25} color 0 selection_color 0 labelsize 11 align 276
    }
  }
  Fl_Group groupFormat {
    xywh {80 40 300 195} hide
  } {
    Fl_Box boxFormatTitle {
      xywh {90 45 265 25} color 0 selection_color 0 labelfont 1 labelsize 11 labelcolor 171 align 276
      code0 {o->image (get_icon_warning());}
    }
    Fl_Button buttonFormatCancel {
      label Cancelar
      callback OnButtonEvent
      xywh {290 195 80 25} box FLAT_BOX down_box FLAT_BOX color 7 selection_color 171 labelfont 1 labelsize 11 labelcolor 37 align 256
      code0 {o->image (get_icon_no());}
    }
    Fl_Box boxFormatInfo {
      xywh {90 125 265 50} box FLAT_BOX color 7 selection_color 0 labelsize 11 align 404
    }
    Fl_Progress formatProgress {
      xywh {90 180 270 10} box FLAT_BOX selection_color 171 labelsize 11
    }
  }
}

widget_class UIInfo {
  label {Información}
  xywh {499 257 335 365} type Double color 7 labelsize 11 hide
  class Fl_Double_Window modal
} {
  Fl_Output outPath {
    label { Path :}
    xywh {100 39 211 24} box NO_BOX selection_color 171 labelsize 11 align 260 textfont 5 textsize 11 textcolor 1 deactivate
    code0 {o->image (get_icon_warning());}
  }
  Fl_Output outVendor {
    label {Vendor :}
    xywh {100 92 211 24} box NO_BOX selection_color 171 labelsize 11 textsize 11 deactivate
  }
  Fl_Output outProduct {
    label {Product :}
    xywh {100 121 211 24} box NO_BOX selection_color 171 labelsize 11 textsize 11 deactivate
  }
  Fl_Output outIdProduct {
    label {ID Product :}
    xywh {100 150 211 24} box NO_BOX selection_color 171 labelsize 11 textsize 11 deactivate
  }
  Fl_Output outManufacturer {
    label {Manufacturer :}
    xywh {100 179 211 24} box NO_BOX selection_color 171 labelsize 11 textsize 11 deactivate
  }
  Fl_Output outSerial {
    label {Serial :}
    xywh {100 208 211 24} box NO_BOX selection_color 171 labelsize 11 textsize 11 deactivate
  }
  Fl_Output outVersion {
    label {Version :}
    xywh {100 237 211 24} box NO_BOX selection_color 171 labelsize 11 textsize 11 deactivate
  }
  Fl_Output outPower {
    label {Power max. :}
    xywh {100 266 211 24} box NO_BOX color 54 selection_color 171 labelsize 11 textsize 11 textcolor 34 deactivate
  }
  Fl_Output outBus {
    label {Bus :}
    xywh {100 295 211 24} box NO_BOX color 55 selection_color 171 labelsize 11 textsize 11 textcolor 34 deactivate
  }
  Fl_Button buttonCloseInfo {
    label Cerrar
    callback OnButtonEvent
    xywh {232 329 80 25} box FLAT_BOX down_box FLAT_BOX color 7 selection_color 171 labelfont 1 labelsize 11 labelcolor 37 align 256
    code0 {o->image (get_icon_no());}
  }
  Fl_Button buttonSaveInfo {
    label {Guardar...}
    callback OnButtonEvent
    xywh {147 329 80 25} box FLAT_BOX down_box FLAT_BOX color 7 selection_color 171 labelfont 1 labelsize 11 labelcolor 37 align 256
    code0 {o->image (get_icon_save());}
  }
  Fl_Button buttonNextInfo {
    callback OnButtonEvent
    xywh {71 329 35 25} box FLAT_BOX down_box FLAT_BOX color 7 selection_color 171 labelfont 1 labelsize 11 labelcolor 37 align 256
    code0 {o->image (get_icon_next());}
  }
  Fl_Button buttonPrevInfo {
    callback OnButtonEvent
    xywh {36 329 35 25} box FLAT_BOX down_box FLAT_BOX color 7 selection_color 171 labelfont 1 labelsize 11 labelcolor 37 align 256
    code0 {o->image (get_icon_previous());}
  }
  Fl_Box outTitle {
    xywh {20 0 310 34} box FLAT_BOX color 7 labelfont 1 labelsize 11 labelcolor 171 align 276
    code0 {o->image (get_icon_list());}
  }
  Fl_Output outSize {
    label { Size :}
    xywh {100 63 211 24} box NO_BOX color 55 selection_color 171 labelsize 11 align 260 textfont 5 textsize 11 textcolor 1 deactivate
    code0 {o->image (get_icon_warning());}
  }
}

widget_class UIMsg {open
  xywh {441 335 355 115} type Double color 7 hide
  class Fl_Double_Window modal
} {
  decl {enum {NO=0, YES=1};} {public local
  }
  Fl_Box boxMsg {
    protected xywh {15 0 330 80} selection_color 171 labelsize 11 align 276
  }
  Fl_Button buttonNo {
    label No
    user_data this
    callback OnEvent
    protected xywh {180 85 80 25} box FLAT_BOX down_box FLAT_BOX color 7 selection_color 171 labelfont 1 labelsize 11 labelcolor 37 align 256
  }
  Fl_Button buttonYes {
    label Si
    user_data this
    callback OnEvent
    protected xywh {265 85 80 25} box FLAT_BOX down_box FLAT_BOX color 7 selection_color 171 labelfont 1 labelsize 11 labelcolor 37 align 256
  }
  Function {OnEvent(Fl_Button * button, void * data)} {open protected return_type {static void}
  } {
    code {UIMsg * msg = (UIMsg*)data;

msg->reply = (button == msg->buttonYes) ? YES : NO;
msg->hide ();} {selected
    }
  }
  Function {Error(char const * title, char const * msg, char const * yes)} {open return_type void
  } {
    code {buttonNo->hide ();
this->title = title;
this->msg = msg;
this->yes = yes;
SetIcons (get_icon_error (), get_icon_yes_activate());//, get_icon_no());
SetText ();} {}
  }
  Function {Question(char const * title, char const  * msg, char const * yes, char const * no)} {open return_type void
  } {
    code {buttonYes->show();
buttonNo->show();
this->title = title;
this->msg = msg;
this->yes = yes;
this->no = no;
SetIcons (get_icon_question (), get_icon_yes_activate(), get_icon_no());
SetText ();} {}
  }
  Function {QuestionWarning(char const * title, char const  * msg, char const * yes, char const * no)} {open return_type void
  } {
    code {buttonYes->show();
buttonNo->show();
this->title = title;
this->msg = msg;
this->yes = yes;
this->no = no;
SetIcons (get_icon_big_warning (), get_icon_yes_activate(), get_icon_no());
SetText ();} {}
  }
  Function {Information(char const * title, char const * msg, char const * yes)} {open return_type void
  } {
    code {buttonYes->show();
buttonNo->hide ();
this->title = title;
this->msg = msg;
this->yes = yes;

SetIcons (get_icon_information (), get_icon_yes_activate());//, get_icon_no());
SetText ();} {}
  }
  Function {SetIcons(Fl_Image * icon, Fl_Image * iyes , Fl_Image * ino = nullptr)} {open protected return_type void
  } {
    code {boxMsg->image (icon);
buttonYes->image (iyes);
buttonNo->image (ino);} {}
  }
  Function {SetText()} {open protected return_type void
  } {
    code {this->label (title);
boxMsg->label (msg);
buttonYes->label (yes);
buttonNo->label (no);} {}
  }
  decl {char const * title;} {private local
  }
  decl {char const * msg;} {private local
  }
  decl {char const * yes;} {private local
  }
  decl {char const * no;} {private local
  }
  decl {unsigned char reply;} {public local
  }
}
