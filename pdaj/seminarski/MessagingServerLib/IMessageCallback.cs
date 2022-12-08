using System;
using System.Collections.Generic;
using System.Linq;
using System.ServiceModel;
using System.Text;
using System.Threading.Tasks;

namespace MessagingServerLib
{
    public interface IMessageCallback
    {
        [OperationContract(IsOneWay = true)]
        void OnMessage(Message m);

        [OperationContract(IsOneWay = true)]
        void OnInformation(string information);

        [OperationContract(IsOneWay = true)]
        void OnError(string error);
    }
}
