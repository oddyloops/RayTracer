using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations.Schema;
using System.Text;

namespace RTDataAccess.DataObjects.SqlAzure
{
    [Table("RTUser")]
    public class RTSqlAzureUser : RTUser
    {
       [Column("User_Id")]
        public override Guid Id { get => base.Id; set => base.Id = value; }
        [Column("Username")]
        public override string UserName { get => base.UserName; set => base.UserName = value; }
        [Column("Password")]
        public override byte[] Password { get => base.Password; set => base.Password = value; }
        [Column("Email")]
        public override string Email { get => base.Email; set => base.Email = value; }
    }
}
