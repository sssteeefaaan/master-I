import { NgModule } from '@angular/core';
import { RouterModule, Route } from '@angular/router';
import { ArtikalListComponent } from './components/artikal-components/artikal-list/artikal-list.component';
import { ArtikalAddComponent } from './components/artikal-components/artikal-add/artikal-add.component';
import { ArtikalEditComponent } from './components/artikal-components/artikal-edit/artikal-edit.component';
import { LoginComponent } from './components/login/login.component';
import { KorpaComponent } from './components/artikal-components/korpa/korpa.component';

import { UserFilterService } from './services/filters/user-filter.service';
import { AdminFilterService } from './services/filters/admin-filter.service';
import { KupacFilterService } from './services/filters/kupac-filter.service';

const routes: Route[] = [
  { path: 'login', component: LoginComponent},
  { path: 'artikli', component: ArtikalListComponent, canActivate : [UserFilterService]},
  { path: 'korpa', component: KorpaComponent, canActivate : [KupacFilterService]},
  { path: 'add', component: ArtikalAddComponent, canActivate : [AdminFilterService]},
  { path: 'edit/:id', component: ArtikalEditComponent, canActivate : [AdminFilterService]},
  { path: '', redirectTo: '/login', pathMatch: 'full'}
];

@NgModule({
  imports: [
    RouterModule.forRoot(routes)
  ],
  exports: [
    RouterModule
  ]
})
export class AppRoutingModule { }
