
using RTMeld.Attributes;

namespace RTMeld.DataTransport
{

    public interface IRTFileType
    {
        [Map("id")]
        int Id { get; set; }

        [Map("extension,ext")]
        string Ext { get; set; }

        [Map("full_name,Fullname,fullname")]
        string FullName { get; set; }
    }
}
