using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations.Schema;
using System.Text;

namespace RTDataAccess.DataObjects.SqlAzure
{
    [Table("RTUserSession")]
    public class RTSqlAzureUserSession : RTUserSession
    {
        [Column("UserId")]
        public override Guid UserId { get => base.UserId; set => base.UserId = value; }
        [Column("SessionId")]
        public override byte[] SessionId { get => base.SessionId; set => base.SessionId = value; }
        [Column("LogInTime")]
        public override DateTime LogInTime { get => base.LogInTime; set => base.LogInTime = value; }
    }
}
