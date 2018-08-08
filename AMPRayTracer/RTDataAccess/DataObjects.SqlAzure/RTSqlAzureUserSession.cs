using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations.Schema;
using System.Text;

namespace RTDataAccess.DataObjects.SqlAzure
{
    [Table("RT_User_Session")]
    public class RTSqlAzureUserSession : RTUserSession
    {
        
        [Column("Id")]
        public override Guid Id { get => base.Id; set => base.Id = value; }

        [Column("UserId")]
        public override Guid UserId { get => base.UserId; set => base.UserId = value; }
        [Column("SessionId")]
        public override byte[] SessionId { get => base.SessionId; set => base.SessionId = value; }
        [Column("LogInTime")]
        public override DateTime LogInTime { get => base.LogInTime; set => base.LogInTime = value; }
    }
}
