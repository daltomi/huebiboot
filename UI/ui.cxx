// generated by Fast Light User Interface Designer (fluid) version 1.0304

#include "ui.hpp"

void ShowWindowModal(Fl_Window * window) {
  window->set_modal ();
  window->show ();
  while (window->shown ())
  {
    Fl::wait ();
  }
}

Fl_Menu_Item UIMain::menu_choiceFormat[] = {
 {"FAT-32", 0,  0, (void*)(FORMATS::FAT), 0, (uchar)FL_NORMAL_LABEL, 0, 11, 0},
 {"NTFS", 0,  0, (void*)(FORMATS::NTFS), 0, (uchar)FL_NORMAL_LABEL, 0, 11, 0},
 {"EXT-3", 0,  0, (void*)(FORMATS::EXT3), 0, (uchar)FL_NORMAL_LABEL, 0, 11, 0},
 {"EXT-4", 0,  0, (void*)(FORMATS::EXT4), 0, (uchar)FL_NORMAL_LABEL, 0, 11, 0},
 {0,0,0,0,0,0,0,0,0}
};
UIMain::UIMain(int X, int Y, int W, int H, const char *L)
  : Fl_Double_Window(X, Y, W, H, L) {
  _UIMain();
}

UIMain::UIMain(int W, int H, const char *L)
  : Fl_Double_Window(0, 0, W, H, L) {
  clear_flag(16);
  _UIMain();
}

UIMain::UIMain()
  : Fl_Double_Window(0, 0, 380, 235, "huebiboot") {
  clear_flag(16);
  _UIMain();
}

void UIMain::_UIMain() {
this->box(FL_FLAT_BOX);
this->color((Fl_Color)55);
this->selection_color(FL_FOREGROUND_COLOR);
this->labeltype(FL_NO_LABEL);
this->labelfont(0);
this->labelsize(14);
this->labelcolor(FL_FOREGROUND_COLOR);
this->align(Fl_Align(FL_ALIGN_TOP));
this->when(FL_WHEN_RELEASE);
{ Fl_Box* o = boxTitle = new Fl_Box(0, 40, 75, 195);
  boxTitle->box(FL_FLAT_BOX);
  boxTitle->color((Fl_Color)55);
  boxTitle->align(Fl_Align(512));
  o->image (get_icon_title());
} // Fl_Box* boxTitle
static int col_widths [] = {80, 80, 0};
{ groupMain = new Fl_Group(0, 0, 416, 235);
  groupMain->hide();
  { Fl_Group* o = new Fl_Group(80, 30, 336, 170);
    o->color(FL_LIGHT1);
    { Fl_Browser* o = browserDevices = new Fl_Browser(90, 43, 281, 93, "Dispositivos");
      browserDevices->type(2);
      browserDevices->box(FL_UP_BOX);
      browserDevices->color((Fl_Color)55);
      browserDevices->selection_color((Fl_Color)171);
      browserDevices->labelsize(11);
      browserDevices->textsize(11);
      browserDevices->callback((Fl_Callback*)OnBrowserEvent);
      browserDevices->align(Fl_Align(FL_ALIGN_TOP_LEFT));
      o->column_widths (col_widths);
      o->column_char ('\t');
    } // Fl_Browser* browserDevices
    { /*
       ¡¡ No cambiar el orden !!
       Debe coincidir con FORMATS
       */
      choiceFormat = new Fl_Choice(90, 166, 281, 20, "Tipo formato (part .#3)");
      choiceFormat->box(FL_THIN_UP_BOX);
      choiceFormat->down_box(FL_THIN_UP_BOX);
      choiceFormat->color(FL_LIGHT1);
      choiceFormat->selection_color((Fl_Color)171);
      choiceFormat->labelsize(11);
      choiceFormat->textsize(11);
      choiceFormat->callback((Fl_Callback*)OnChoiceEvent, (void*)(this));
      choiceFormat->align(Fl_Align(FL_ALIGN_TOP_LEFT));
      choiceFormat->menu(menu_choiceFormat);
    } // Fl_Choice* choiceFormat
    o->end();
  } // Fl_Group* o
  { Fl_Group* o = new Fl_Group(90, 195, 276, 40);
    { Fl_Button* o = buttonFormat = new Fl_Button(195, 195, 80, 25, "Comenzar");
      buttonFormat->box(FL_FLAT_BOX);
      buttonFormat->down_box(FL_FLAT_BOX);
      buttonFormat->color(FL_BACKGROUND2_COLOR);
      buttonFormat->selection_color((Fl_Color)171);
      buttonFormat->labelfont(1);
      buttonFormat->labelsize(11);
      buttonFormat->labelcolor((Fl_Color)37);
      buttonFormat->callback((Fl_Callback*)OnButtonEvent);
      buttonFormat->align(Fl_Align(256));
      o->image (get_icon_yes_activate());
      o->deimage (get_icon_yes_deactivate());
    } // Fl_Button* buttonFormat
    { Fl_Button* o = buttonCancel = new Fl_Button(286, 195, 80, 25, "Cancelar");
      buttonCancel->box(FL_FLAT_BOX);
      buttonCancel->down_box(FL_FLAT_BOX);
      buttonCancel->color(FL_BACKGROUND2_COLOR);
      buttonCancel->selection_color((Fl_Color)171);
      buttonCancel->labelfont(1);
      buttonCancel->labelsize(11);
      buttonCancel->labelcolor((Fl_Color)37);
      buttonCancel->callback((Fl_Callback*)OnButtonEvent);
      buttonCancel->align(Fl_Align(256));
      o->image (get_icon_no());
    } // Fl_Button* buttonCancel
    o->end();
  } // Fl_Group* o
  { Fl_Group* o = new Fl_Group(0, 0, 380, 29);
    { Fl_Button* o = buttonToolInfo = new Fl_Button(5, 5, 24, 24);
      buttonToolInfo->tooltip("Ver informaci\303\263n...");
      buttonToolInfo->box(FL_FLAT_BOX);
      buttonToolInfo->down_box(FL_FLAT_BOX);
      buttonToolInfo->color(FL_BACKGROUND2_COLOR);
      buttonToolInfo->selection_color((Fl_Color)171);
      buttonToolInfo->labelsize(9);
      buttonToolInfo->callback((Fl_Callback*)OnButtonEvent);
      buttonToolInfo->align(Fl_Align(512));
      o->image (get_icon_list());
    } // Fl_Button* buttonToolInfo
    { Fl_Button* o = buttonToolBackup = new Fl_Button(40, 5, 24, 24);
      buttonToolBackup->tooltip("Copia de seguridad...");
      buttonToolBackup->box(FL_FLAT_BOX);
      buttonToolBackup->down_box(FL_FLAT_BOX);
      buttonToolBackup->color(FL_BACKGROUND2_COLOR);
      buttonToolBackup->selection_color((Fl_Color)171);
      buttonToolBackup->labelsize(9);
      buttonToolBackup->callback((Fl_Callback*)OnButtonEvent);
      buttonToolBackup->align(Fl_Align(512));
      o->image (get_icon_backup());
    } // Fl_Button* buttonToolBackup
    { Fl_Button* o = buttonToolHelp = new Fl_Button(346, 5, 24, 24);
      buttonToolHelp->tooltip("Ayuda...");
      buttonToolHelp->box(FL_FLAT_BOX);
      buttonToolHelp->down_box(FL_FLAT_BOX);
      buttonToolHelp->color(FL_BACKGROUND2_COLOR);
      buttonToolHelp->selection_color((Fl_Color)171);
      buttonToolHelp->labelsize(9);
      buttonToolHelp->callback((Fl_Callback*)OnButtonEvent);
      buttonToolHelp->align(Fl_Align(512));
      o->image (get_icon_help());
    } // Fl_Button* buttonToolHelp
    o->end();
  } // Fl_Group* o
  groupMain->end();
} // Fl_Group* groupMain
{ groupSplash = new Fl_Group(80, 55, 300, 180);
  groupSplash->color(FL_LIGHT1);
  { boxSplashWait = new Fl_Box(80, 105, 290, 20, "Por favor, conecte su dispositivo USB ...");
    boxSplashWait->box(FL_FLAT_BOX);
    boxSplashWait->color((Fl_Color)55);
    boxSplashWait->labelsize(11);
    boxSplashWait->align(Fl_Align(FL_ALIGN_TOP_LEFT|FL_ALIGN_INSIDE));
  } // Fl_Box* boxSplashWait
  { Fl_Button* o = buttonQuit = new Fl_Button(290, 195, 80, 25, "Salir");
    buttonQuit->box(FL_FLAT_BOX);
    buttonQuit->down_box(FL_FLAT_BOX);
    buttonQuit->color(FL_BACKGROUND2_COLOR);
    buttonQuit->selection_color((Fl_Color)171);
    buttonQuit->labelfont(1);
    buttonQuit->labelsize(11);
    buttonQuit->labelcolor((Fl_Color)37);
    buttonQuit->callback((Fl_Callback*)OnButtonEvent);
    buttonQuit->align(Fl_Align(256));
    o->image (get_icon_no());
  } // Fl_Button* buttonQuit
  { boxSplashNotFound = new Fl_Box(80, 125, 290, 20, "No se encontr\303\263 ning\303\272n dispositivo, reintentando ...");
    boxSplashNotFound->box(FL_FLAT_BOX);
    boxSplashNotFound->color(FL_BACKGROUND2_COLOR);
    boxSplashNotFound->labelsize(11);
    boxSplashNotFound->labelcolor((Fl_Color)1);
    boxSplashNotFound->align(Fl_Align(FL_ALIGN_TOP_LEFT|FL_ALIGN_INSIDE));
    boxSplashNotFound->hide();
  } // Fl_Box* boxSplashNotFound
  groupSplash->end();
} // Fl_Group* groupSplash
{ groupBackup = new Fl_Group(80, 40, 300, 195);
  groupBackup->hide();
  { Fl_Box* o = boxBackupTitle = new Fl_Box(90, 45, 265, 25, "Copia de seguridad");
    boxBackupTitle->color(FL_FOREGROUND_COLOR);
    boxBackupTitle->selection_color(FL_FOREGROUND_COLOR);
    boxBackupTitle->labelfont(1);
    boxBackupTitle->labelsize(11);
    boxBackupTitle->labelcolor((Fl_Color)171);
    boxBackupTitle->align(Fl_Align(260|FL_ALIGN_INSIDE));
    o->image (get_icon_save());
  } // Fl_Box* boxBackupTitle
  { Fl_Button* o = buttonBackupCancel = new Fl_Button(290, 195, 80, 25, "Cancelar");
    buttonBackupCancel->box(FL_FLAT_BOX);
    buttonBackupCancel->down_box(FL_FLAT_BOX);
    buttonBackupCancel->color(FL_BACKGROUND2_COLOR);
    buttonBackupCancel->selection_color((Fl_Color)171);
    buttonBackupCancel->labelfont(1);
    buttonBackupCancel->labelsize(11);
    buttonBackupCancel->labelcolor((Fl_Color)37);
    buttonBackupCancel->callback((Fl_Callback*)OnButtonEvent);
    buttonBackupCancel->align(Fl_Align(256));
    o->image (get_icon_no());
  } // Fl_Button* buttonBackupCancel
  { boxBackupFile = new Fl_Box(90, 100, 265, 25, "Archivo: ");
    boxBackupFile->color(FL_FOREGROUND_COLOR);
    boxBackupFile->selection_color(FL_FOREGROUND_COLOR);
    boxBackupFile->labelsize(11);
    boxBackupFile->align(Fl_Align(260|FL_ALIGN_INSIDE));
  } // Fl_Box* boxBackupFile
  { boxBackupBytes = new Fl_Box(90, 125, 265, 25);
    boxBackupBytes->color(FL_FOREGROUND_COLOR);
    boxBackupBytes->selection_color(FL_FOREGROUND_COLOR);
    boxBackupBytes->labelsize(11);
    boxBackupBytes->align(Fl_Align(260|FL_ALIGN_INSIDE));
  } // Fl_Box* boxBackupBytes
  groupBackup->end();
} // Fl_Group* groupBackup
{ groupFormat = new Fl_Group(80, 40, 300, 195);
  groupFormat->hide();
  { Fl_Box* o = boxFormatTitle = new Fl_Box(90, 45, 265, 25);
    boxFormatTitle->color(FL_FOREGROUND_COLOR);
    boxFormatTitle->selection_color(FL_FOREGROUND_COLOR);
    boxFormatTitle->labelfont(1);
    boxFormatTitle->labelsize(11);
    boxFormatTitle->labelcolor((Fl_Color)171);
    boxFormatTitle->align(Fl_Align(260|FL_ALIGN_INSIDE));
    o->image (get_icon_warning());
  } // Fl_Box* boxFormatTitle
  { Fl_Button* o = buttonFormatCancel = new Fl_Button(290, 195, 80, 25, "Cancelar");
    buttonFormatCancel->box(FL_FLAT_BOX);
    buttonFormatCancel->down_box(FL_FLAT_BOX);
    buttonFormatCancel->color(FL_BACKGROUND2_COLOR);
    buttonFormatCancel->selection_color((Fl_Color)171);
    buttonFormatCancel->labelfont(1);
    buttonFormatCancel->labelsize(11);
    buttonFormatCancel->labelcolor((Fl_Color)37);
    buttonFormatCancel->callback((Fl_Callback*)OnButtonEvent);
    buttonFormatCancel->align(Fl_Align(256));
    o->image (get_icon_no());
  } // Fl_Button* buttonFormatCancel
  { boxFormatInfo = new Fl_Box(90, 125, 265, 50);
    boxFormatInfo->box(FL_FLAT_BOX);
    boxFormatInfo->color(FL_BACKGROUND2_COLOR);
    boxFormatInfo->selection_color(FL_FOREGROUND_COLOR);
    boxFormatInfo->labelsize(11);
    boxFormatInfo->align(Fl_Align(388|FL_ALIGN_INSIDE));
  } // Fl_Box* boxFormatInfo
  { formatProgress = new Fl_Progress(90, 180, 270, 10);
    formatProgress->box(FL_FLAT_BOX);
    formatProgress->selection_color((Fl_Color)171);
    formatProgress->labelsize(11);
  } // Fl_Progress* formatProgress
  groupFormat->end();
} // Fl_Group* groupFormat
fl_register_images();
selectFormat = FORMATS::FAT;
set_modal();
end();
}

void UIMain::OnChoiceEvent(Fl_Choice *choice, void *data) {
  UIMain * ui = (UIMain*)(data);
  
  ui->selectFormat = (UIMain::FORMATS)choice->value();
}
UIInfo::UIInfo(int X, int Y, int W, int H, const char *L)
  : Fl_Double_Window(X, Y, W, H, L) {
  _UIInfo();
}

UIInfo::UIInfo(int W, int H, const char *L)
  : Fl_Double_Window(0, 0, W, H, L) {
  clear_flag(16);
  _UIInfo();
}

UIInfo::UIInfo()
  : Fl_Double_Window(0, 0, 335, 365, "Informaci\303\263n") {
  clear_flag(16);
  _UIInfo();
}

void UIInfo::_UIInfo() {
this->box(FL_FLAT_BOX);
this->color(FL_BACKGROUND2_COLOR);
this->selection_color(FL_BACKGROUND_COLOR);
this->labeltype(FL_NO_LABEL);
this->labelfont(0);
this->labelsize(11);
this->labelcolor(FL_FOREGROUND_COLOR);
this->align(Fl_Align(FL_ALIGN_TOP));
this->when(FL_WHEN_RELEASE);
{ Fl_Output* o = outPath = new Fl_Output(100, 39, 211, 24, " Path :");
  outPath->box(FL_NO_BOX);
  outPath->selection_color((Fl_Color)171);
  outPath->labelsize(11);
  outPath->textfont(5);
  outPath->textsize(11);
  outPath->textcolor((Fl_Color)1);
  outPath->align(Fl_Align(260));
  outPath->deactivate();
  o->image (get_icon_warning());
} // Fl_Output* outPath
{ outVendor = new Fl_Output(100, 92, 211, 24, "Vendor :");
  outVendor->box(FL_NO_BOX);
  outVendor->selection_color((Fl_Color)171);
  outVendor->labelsize(11);
  outVendor->textsize(11);
  outVendor->deactivate();
} // Fl_Output* outVendor
{ outProduct = new Fl_Output(100, 121, 211, 24, "Product :");
  outProduct->box(FL_NO_BOX);
  outProduct->selection_color((Fl_Color)171);
  outProduct->labelsize(11);
  outProduct->textsize(11);
  outProduct->deactivate();
} // Fl_Output* outProduct
{ outIdProduct = new Fl_Output(100, 150, 211, 24, "ID Product :");
  outIdProduct->box(FL_NO_BOX);
  outIdProduct->selection_color((Fl_Color)171);
  outIdProduct->labelsize(11);
  outIdProduct->textsize(11);
  outIdProduct->deactivate();
} // Fl_Output* outIdProduct
{ outManufacturer = new Fl_Output(100, 179, 211, 24, "Manufacturer :");
  outManufacturer->box(FL_NO_BOX);
  outManufacturer->selection_color((Fl_Color)171);
  outManufacturer->labelsize(11);
  outManufacturer->textsize(11);
  outManufacturer->deactivate();
} // Fl_Output* outManufacturer
{ outSerial = new Fl_Output(100, 208, 211, 24, "Serial :");
  outSerial->box(FL_NO_BOX);
  outSerial->selection_color((Fl_Color)171);
  outSerial->labelsize(11);
  outSerial->textsize(11);
  outSerial->deactivate();
} // Fl_Output* outSerial
{ outVersion = new Fl_Output(100, 237, 211, 24, "Version :");
  outVersion->box(FL_NO_BOX);
  outVersion->selection_color((Fl_Color)171);
  outVersion->labelsize(11);
  outVersion->textsize(11);
  outVersion->deactivate();
} // Fl_Output* outVersion
{ outPower = new Fl_Output(100, 266, 211, 24, "Power max. :");
  outPower->box(FL_NO_BOX);
  outPower->color(FL_LIGHT3);
  outPower->selection_color((Fl_Color)171);
  outPower->labelsize(11);
  outPower->textsize(11);
  outPower->textcolor((Fl_Color)34);
  outPower->deactivate();
} // Fl_Output* outPower
{ outBus = new Fl_Output(100, 295, 211, 24, "Bus :");
  outBus->box(FL_NO_BOX);
  outBus->color((Fl_Color)55);
  outBus->selection_color((Fl_Color)171);
  outBus->labelsize(11);
  outBus->textsize(11);
  outBus->textcolor((Fl_Color)34);
  outBus->deactivate();
} // Fl_Output* outBus
{ Fl_Button* o = buttonCloseInfo = new Fl_Button(232, 329, 80, 25, "Cerrar");
  buttonCloseInfo->box(FL_FLAT_BOX);
  buttonCloseInfo->down_box(FL_FLAT_BOX);
  buttonCloseInfo->color(FL_BACKGROUND2_COLOR);
  buttonCloseInfo->selection_color((Fl_Color)171);
  buttonCloseInfo->labelfont(1);
  buttonCloseInfo->labelsize(11);
  buttonCloseInfo->labelcolor((Fl_Color)37);
  buttonCloseInfo->callback((Fl_Callback*)OnButtonEvent);
  buttonCloseInfo->align(Fl_Align(256));
  o->image (get_icon_no());
} // Fl_Button* buttonCloseInfo
{ Fl_Button* o = buttonSaveInfo = new Fl_Button(147, 329, 80, 25, "Guardar...");
  buttonSaveInfo->box(FL_FLAT_BOX);
  buttonSaveInfo->down_box(FL_FLAT_BOX);
  buttonSaveInfo->color(FL_BACKGROUND2_COLOR);
  buttonSaveInfo->selection_color((Fl_Color)171);
  buttonSaveInfo->labelfont(1);
  buttonSaveInfo->labelsize(11);
  buttonSaveInfo->labelcolor((Fl_Color)37);
  buttonSaveInfo->callback((Fl_Callback*)OnButtonEvent);
  buttonSaveInfo->align(Fl_Align(256));
  o->image (get_icon_save());
} // Fl_Button* buttonSaveInfo
{ Fl_Button* o = buttonNextInfo = new Fl_Button(71, 329, 35, 25);
  buttonNextInfo->box(FL_FLAT_BOX);
  buttonNextInfo->down_box(FL_FLAT_BOX);
  buttonNextInfo->color(FL_BACKGROUND2_COLOR);
  buttonNextInfo->selection_color((Fl_Color)171);
  buttonNextInfo->labelfont(1);
  buttonNextInfo->labelsize(11);
  buttonNextInfo->labelcolor((Fl_Color)37);
  buttonNextInfo->callback((Fl_Callback*)OnButtonEvent);
  buttonNextInfo->align(Fl_Align(256));
  o->image (get_icon_next());
} // Fl_Button* buttonNextInfo
{ Fl_Button* o = buttonPrevInfo = new Fl_Button(36, 329, 35, 25);
  buttonPrevInfo->box(FL_FLAT_BOX);
  buttonPrevInfo->down_box(FL_FLAT_BOX);
  buttonPrevInfo->color(FL_BACKGROUND2_COLOR);
  buttonPrevInfo->selection_color((Fl_Color)171);
  buttonPrevInfo->labelfont(1);
  buttonPrevInfo->labelsize(11);
  buttonPrevInfo->labelcolor((Fl_Color)37);
  buttonPrevInfo->callback((Fl_Callback*)OnButtonEvent);
  buttonPrevInfo->align(Fl_Align(256));
  o->image (get_icon_previous());
} // Fl_Button* buttonPrevInfo
{ Fl_Box* o = outTitle = new Fl_Box(20, 0, 310, 34);
  outTitle->box(FL_FLAT_BOX);
  outTitle->color(FL_BACKGROUND2_COLOR);
  outTitle->labelfont(1);
  outTitle->labelsize(11);
  outTitle->labelcolor((Fl_Color)171);
  outTitle->align(Fl_Align(260|FL_ALIGN_INSIDE));
  o->image (get_icon_list());
} // Fl_Box* outTitle
{ Fl_Output* o = outSize = new Fl_Output(100, 63, 211, 24, " Size :");
  outSize->box(FL_NO_BOX);
  outSize->color((Fl_Color)55);
  outSize->selection_color((Fl_Color)171);
  outSize->labelsize(11);
  outSize->textfont(5);
  outSize->textsize(11);
  outSize->textcolor((Fl_Color)1);
  outSize->align(Fl_Align(260));
  outSize->deactivate();
  o->image (get_icon_warning());
} // Fl_Output* outSize
set_modal();
end();
}
UIMsg::UIMsg(int X, int Y, int W, int H, const char *L)
  : Fl_Double_Window(X, Y, W, H, L) {
  _UIMsg();
}

UIMsg::UIMsg(int W, int H, const char *L)
  : Fl_Double_Window(0, 0, W, H, L) {
  clear_flag(16);
  _UIMsg();
}

UIMsg::UIMsg()
  : Fl_Double_Window(0, 0, 355, 115, 0) {
  clear_flag(16);
  _UIMsg();
}

void UIMsg::_UIMsg() {
this->box(FL_FLAT_BOX);
this->color(FL_BACKGROUND2_COLOR);
this->selection_color(FL_BACKGROUND_COLOR);
this->labeltype(FL_NO_LABEL);
this->labelfont(0);
this->labelsize(14);
this->labelcolor(FL_FOREGROUND_COLOR);
this->align(Fl_Align(FL_ALIGN_TOP));
this->when(FL_WHEN_RELEASE);
{ boxMsg = new Fl_Box(15, 0, 330, 80);
  boxMsg->selection_color((Fl_Color)171);
  boxMsg->labelsize(11);
  boxMsg->align(Fl_Align(260|FL_ALIGN_INSIDE));
} // Fl_Box* boxMsg
{ buttonNo = new Fl_Button(180, 85, 80, 25, "No");
  buttonNo->box(FL_FLAT_BOX);
  buttonNo->down_box(FL_FLAT_BOX);
  buttonNo->color(FL_BACKGROUND2_COLOR);
  buttonNo->selection_color((Fl_Color)171);
  buttonNo->labelfont(1);
  buttonNo->labelsize(11);
  buttonNo->labelcolor((Fl_Color)37);
  buttonNo->callback((Fl_Callback*)OnEvent, (void*)(this));
  buttonNo->align(Fl_Align(256));
} // Fl_Button* buttonNo
{ buttonYes = new Fl_Button(265, 85, 80, 25, "Si");
  buttonYes->box(FL_FLAT_BOX);
  buttonYes->down_box(FL_FLAT_BOX);
  buttonYes->color(FL_BACKGROUND2_COLOR);
  buttonYes->selection_color((Fl_Color)171);
  buttonYes->labelfont(1);
  buttonYes->labelsize(11);
  buttonYes->labelcolor((Fl_Color)37);
  buttonYes->callback((Fl_Callback*)OnEvent, (void*)(this));
  buttonYes->align(Fl_Align(256));
} // Fl_Button* buttonYes
set_modal();
end();
}

void UIMsg::OnEvent(Fl_Button * button, void * data) {
  UIMsg * msg = (UIMsg*)data;
  
  msg->reply = (button == msg->buttonYes) ? YES : NO;
  msg->hide ();
}

void UIMsg::Error(char const * title, char const * msg, char const * yes) {
  buttonNo->hide ();
  this->title = title;
  this->msg = msg;
  this->yes = yes;
  SetIcons (get_icon_error (), get_icon_yes_activate());//, get_icon_no());
  SetText ();
}

void UIMsg::Question(char const * title, char const  * msg, char const * yes, char const * no) {
  buttonYes->show();
  buttonNo->show();
  this->title = title;
  this->msg = msg;
  this->yes = yes;
  this->no = no;
  SetIcons (get_icon_question (), get_icon_yes_activate(), get_icon_no());
  SetText ();
}

void UIMsg::QuestionWarning(char const * title, char const  * msg, char const * yes, char const * no) {
  buttonYes->show();
  buttonNo->show();
  this->title = title;
  this->msg = msg;
  this->yes = yes;
  this->no = no;
  SetIcons (get_icon_big_warning (), get_icon_yes_activate(), get_icon_no());
  SetText ();
}

void UIMsg::Information(char const * title, char const * msg, char const * yes) {
  buttonYes->show();
  buttonNo->hide ();
  this->title = title;
  this->msg = msg;
  this->yes = yes;
  
  SetIcons (get_icon_information (), get_icon_yes_activate());//, get_icon_no());
  SetText ();
}

void UIMsg::SetIcons(Fl_Image * icon, Fl_Image * iyes , Fl_Image * ino ) {
  boxMsg->image (icon);
  buttonYes->image (iyes);
  buttonNo->image (ino);
}

void UIMsg::SetText() {
  this->label (title);
  boxMsg->label (msg);
  buttonYes->label (yes);
  buttonNo->label (no);
}
