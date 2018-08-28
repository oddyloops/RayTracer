
using RTMeld.Attributes;
using System;

namespace RTMeld.DataTransport
{

    public interface IRTFileType : IData
    {
   
        [Key]
        [Map("id,ID")]
        string Id { get; set; }

        [Map("extension,ext")]
        string Ext { get; set; }

        [Map("full_name,Fullname,fullname")]
        string FullName { get; set; }
    }
}
