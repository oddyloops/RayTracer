using RTMeld.Services;
using System;
using System.Collections.Generic;
using System.Text;
using System.Composition;
using RTMeld.Enums;
using System.Threading.Tasks;
using System.Net.Mail;
using RTMeld.DataAccess;
using RTMeld.Config;
using System.Net;
using System.IO;
using System.Linq;
using SendGrid;
using SendGrid.Helpers.Mail;

namespace RTServices
{
    [Export(typeof(ICommunicationService))]
    public class RTCommunicationService : ICommunicationService
    {
        [Import("JsonConfig")]
        public IConnectionContext ConfigContext { get; set; }

        [Import]
        public ISecurityService SecurityService { get; set; }

        [Import]
        public IKeyStoreService KeyStoreService { get; set; }

        public async Task<StatusCode> SendEmailAsync(string sender, string recipient, string subject, string body)
        {
            return await SendEmailAsync(sender, new List<string>() { recipient }, null, null, subject, body);
        }

        public async Task<StatusCode> SendEmailAsync(string sender, IList<string> recipients, IList<string> ccs, IList<string> bccs, string subject, string body)
        {
            
            byte[] encryptedCredentials = File.ReadAllBytes(ConfigContext.GetAppSetting(Config.SEND_GRID_ENCRYPTED));
            byte[] symmKey = KeyStoreService.GetKey(ConfigContext.GetAppSetting(Config.SYMMETRIC_KEY_INDEX));
            string apiKey = Encoding.UTF8.GetString(SecurityService.Decrypt(encryptedCredentials, symmKey));
           
            List<EmailAddress> receivers = (from r in recipients select new EmailAddress(r)).ToList();

            List<EmailAddress> copies = (ccs == null ? new List<EmailAddress>() :
                (from c in ccs select new EmailAddress(c)).ToList());
            List<EmailAddress> blindCopies = (bccs == null ? new List<EmailAddress>() :
                (from b in bccs select new EmailAddress(b)).ToList());

            EmailAddress senderAddress = new EmailAddress(sender);

            SendGridMessage message = new SendGridMessage();
            message.Subject = subject;
            message.SetFrom(senderAddress);
            message.AddTos(receivers);
            message.AddCcs(copies);
            message.AddBccs(blindCopies);
            message.AddContent(MimeType.Html, body);
            
            SendGridClient client = new SendGridClient(apiKey);
            await client.SendEmailAsync(message);
            return StatusCode.Successful;

        }
    }
}
