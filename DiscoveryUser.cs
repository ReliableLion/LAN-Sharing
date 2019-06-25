using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace LanSharing
{
    class DiscoveryUser {

        private static readonly DiscoveryUser instance = new DiscoveryUser();

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate void DiscoveryDelegate(string ipAddress, string username, string avatar);
        
        [DllImport(Constants.DLL_PATH, CallingConvention = CallingConvention.Cdecl)]
        public static extern void save_discovery_callback(DiscoveryDelegate callback);

        [DllImport(Constants.DLL_PATH, CallingConvention = CallingConvention.Cdecl)]
        public static extern void start_collection();

        [DllImport(Constants.DLL_PATH, CallingConvention = CallingConvention.Cdecl)]
        public static extern void hide_me();

        [DllImport(Constants.DLL_PATH, CallingConvention = CallingConvention.Cdecl)]
        public static extern void go_online();

        [DllImport(Constants.DLL_PATH, CallingConvention = CallingConvention.Cdecl)]
        public static extern void set_username(string username);

        [DllImport(Constants.DLL_PATH, CallingConvention = CallingConvention.Cdecl)]
        public static extern void set_avatar(string avatar);

        private static DiscoveryDelegate _discoveryDelegate;
        private readonly SortedDictionary<string, User> _users = new SortedDictionary<string, User>();

        // Explicit static constructor to tell C# compiler
        // not to mark type as beforefieldinit
        static DiscoveryUser() {
        }

        private DiscoveryUser() {
            Save_user();
        }

        public static DiscoveryUser Instance {
            get
            {
                return instance;
            }
        }

        private void Save_user() {

            _discoveryDelegate = delegate(string ipAddress, string username, string avatar)
            {
                if (username == "" || avatar == "") return;

                var user = new User(username, avatar);

                if (!_users.ContainsKey(ipAddress))
                    _users.Add(ipAddress, user);
                else
                    _users[ipAddress] = user;
            };

            save_discovery_callback(_discoveryDelegate);
        }

        public void search_user() {
            _users.Clear();
            start_collection();
        }

        public SortedDictionary<string, User> get_users() {
            return _users;
        }

        public void Hide() {
            hide_me();
        }

        public void Online() {
            go_online();
        }

        public void SetUsername(string username) {
            set_username(username);
        }

        public void SetAvatar(string image_name) {
            set_avatar(image_name);
        }

        public void print() {
            foreach (var user in _users) {
                Console.Out.WriteLine("KEY: " + user.Key + " VALUE: " + user.Value);
            }

            //foreach (var user in _usersImage) {
            //    Console.Out.WriteLine("KEY: " + user.Key + " VALUE: " + user.Value);
            //}
        }

    }
}
