using RTMeld.DataTransport;
using System;
using System.Collections.Generic;
using System.Text;

namespace RTDataAccess.DataObjects
{
    public class RTUserSession : IRTUserSession
    {
        private Guid id;
        private Guid userId;
        private byte[] sessionId;
        private DateTime logInTime;
        
        public virtual Guid Id { get => id; set => id = value; }
        public virtual Guid UserId { get => userId; set => userId = value; }
        public virtual byte[] SessionId { get => sessionId; set => sessionId = value; }
        public virtual DateTime LogInTime { get => logInTime; set => logInTime = value; }

       
    }
}
