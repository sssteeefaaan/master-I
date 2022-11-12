import { Component, OnInit } from '@angular/core';

import { MessageService } from '../../../services/message.service';

@Component({
  selector: 'app-message-list',
  templateUrl: './message-list.component.html',
  styleUrls: ['./message-list.component.css']
})
export class MessageListComponent implements OnInit {

  constructor(private messageService: MessageService) { }

  ngOnInit() {
  }

  obrisiPoruke() {
    if (confirm("Da li ste sigurni da želite da obrišete log poruke?")) {
      this.messageService.clear();
      alert('Log poruke obrisane!');
    }
  }

}
