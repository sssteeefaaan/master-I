import { BrowserModule } from '@angular/platform-browser';
import { NgModule } from '@angular/core';
import { FormsModule } from '@angular/forms';
import { HttpClientModule } from '@angular/common/http';

import { AppComponent } from './app.component';
import { ArtikalListComponent } from './components/artikal-components/artikal-list/artikal-list.component';
import { ArtikalItemComponent } from './components/artikal-components/artikal-item/artikal-item.component';
import { ArtikalAddComponent } from './components/artikal-components/artikal-add/artikal-add.component';
import { ArtikalEditComponent } from './components/artikal-components/artikal-edit/artikal-edit.component';
import { ArtikalService } from './services/artikal.service';
import { MessageService } from './services/message.service';
import { MessageListComponent } from './components/message-components/message-list/message-list.component';
import { AppRoutingModule } from './/app-routing.module';
import { LoginComponent } from './components/login/login.component';
import { KorpaComponent } from './components/artikal-components/korpa/korpa.component';



@NgModule({
  declarations: [
    AppComponent,
    ArtikalListComponent,
    ArtikalItemComponent,
    ArtikalAddComponent,
    MessageListComponent,
    ArtikalEditComponent,
    LoginComponent,
    KorpaComponent
  ],
  imports: [
    BrowserModule,
    FormsModule,
    HttpClientModule,
    AppRoutingModule
  ],
  providers: [ArtikalService, MessageService],
  bootstrap: [AppComponent]
})
export class AppModule { }
