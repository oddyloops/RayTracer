using System;
using System.Collections.Generic;
using RTMeld.Attributes;
using RTMeld.DataAccess;


namespace RTMeld.DataTransport
{
    public interface IRTUser
    {
        [Key]
        [Map("id,user_id,UserId")]
        Guid Id { get; set; }

        [Map("user_name,Username,username")]
        string UserName { get; set; }

        [Map("password,pwd,Pwd")]
        byte[] Password { get; set; }

        [Map("email")]
        string Email { get; set; }

        

    }
}
