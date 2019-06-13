using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace LanSharing
{
    class DiscoveryUser {

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate void DiscoveryDelegate(string ipAddress, string username, string avatar);
        
        [DllImport("C:\\Users\\Asus\\source\\repos\\Dll1\\Debug\\DLL1.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void SaveUserDelegate(DiscoveryDelegate callback);

        private static DiscoveryDelegate _discoveryDelegate;
        private readonly SortedDictionary<string, string> _users = new SortedDictionary<string, string>();
        private readonly SortedDictionary<string, string> _usersImage = new SortedDictionary<string, string>();

        public DiscoveryUser() {
            Save_user();
        }

        private void Save_user() {

            _discoveryDelegate = delegate(string ipAddress, string username, string avatar)
            {
                
                if (username != "")
                {
                    if (!_users.ContainsKey(ipAddress))
                        _users.Add(ipAddress, username);
                    else
                        _users[ipAddress] = username;
                }
                else if (avatar != "") {
                    if (!_usersImage.ContainsKey(ipAddress))
                        _usersImage.Add(ipAddress, avatar);
                    else
                        _usersImage[ipAddress] = avatar;
                }
            };

            SaveUserDelegate(_discoveryDelegate);
        }

        public void print() {
            foreach (var user in _users) {
                Console.Out.WriteLine("KEY: " + user.Key + " VALUE: " + user.Value);
            }

            foreach (var user in _usersImage) {
                Console.Out.WriteLine("KEY: " + user.Key + " VALUE: " + user.Value);
            }
        }

    }
}
