using RTDataAccess.DataObjects;
using RTMeld.DataTransport;
using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations.Schema;
using System.Composition;
using System.Text;

namespace RTDataAccess.SqlAzure.DataObjects
{
    [Table("RT_User")]
    [Export(typeof(IRTUser))]
    public class RTSqlAzureUser : RTUser
    {
        [Column("UserId")]
        public override Guid Id { get => base.Id; set => base.Id = value; }
        [Column("Username")]
        public override string UserName { get => base.UserName; set => base.UserName = value; }
        [Column("Pwd")]
        public override byte[] Password { get => base.Password; set => base.Password = value; }
        [Column("Salt")]
        public override byte[] Salt { get => base.Salt; set => base.Salt = value; }
        [Column("Email")]
        public override string Email { get => base.Email; set => base.Email = value; }
        [Column("Status")]
        public override int Status { get => base.Status ; set => base.Status = value; }
        [Column("RecoveryHash")]
        public override byte[] RecoveryHash { get => base.RecoveryHash; set => base.RecoveryHash = value; }

    }
}
