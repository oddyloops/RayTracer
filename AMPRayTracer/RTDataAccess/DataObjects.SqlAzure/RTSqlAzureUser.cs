using RTMeld.DataTransport;
using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations.Schema;
using System.Text;

namespace RTDataAccess.DataObjects.SqlAzure
{
    [Table("RT_User")]
    public class RTSqlAzureUser : RTUser
    {
       [Column("UserId")]
        public override Guid Id { get => base.Id; set => base.Id = value; }
        [Column("Username")]
        public override string UserName { get => base.UserName; set => base.UserName = value; }
        [Column("Pwd")]
        public override byte[] Password { get => base.Password; set => base.Password = value; }
        [Column("Email")]
        public override string Email { get => base.Email; set => base.Email = value; }
        
    }
}
