using System;
using System.Collections.Generic;
using RTMeld.Attributes;
using RTMeld.DataAccess;
using RTMeld.Enums;

namespace RTMeld.DataTransport
{
    public interface IRTUser : IData
    {
        [Key]
        [Map("id,user_id,UserId")]
        Guid Id { get; set; }

        [Unique]
        [Map("user_name,Username,username")]
        string UserName { get; set; }

        [Map("password,pwd,Pwd")]
        byte[] Password { get; set; }

        [Map("salt")]
        byte[] Salt { get; set; }

        [Unique]
        [Map("email")]
        string Email { get; set; }

        [Map("status")]
        int Status { get; set; }
        
        [Map("Recovery_Hash,recovery_hash")]
        byte[] RecoveryHash { get; set; }
        

    }
}
