
using RTMeld.Attributes;

namespace RTMeld.DataTransport
{
    public interface ICosmos
    {
        [Key]
        [Map("id,Id")]
        string Id { get; set; }
    }
}
