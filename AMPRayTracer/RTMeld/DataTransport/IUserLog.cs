using RTMeld.Attributes;
using System;
using System.Collections.Generic;
using System.Text;

namespace RTMeld.DataTransport
{
    /// <summary>
    /// A contract defining properties of user activities trail
    /// </summary>
    public interface IRTUserLog : IData
    {
        [Key]
        [Map("id,ID")]
        string Id { get; set; }

        [Unique]
        [Map("user_id,User_Id,UserID")]
        string UserId { get; set; }

        [Map("Past_Passwords,Past_Pwds,PastPasswords,past_pwds,past_passwords")]
        ICollection<string> PastPwds { get; set; }
    }
}
