using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace bad_apple_server
{
	class Server
	{
		const string ImagePath = @"images";
		const string VideoFile = @"tohou.mp4"; // Bad apple!! original video (03:39 length)
		const int FPS = 30;

		const string PortName = "COM3";

		static void Main(string[] args)
		{
			var files = Directory.EnumerateFiles(ImagePath).ToList();
			var images = files.Select(File.ReadAllBytes).ToArray();

			WMPLib.WindowsMediaPlayer player = new WMPLib.WindowsMediaPlayer();
			player.settings.autoStart = false;
			player.settings.playCount = 1000;
			player.URL = VideoFile;
			Thread.Sleep(1000); // let wmp to finish all the loading shit

			var com = new System.IO.Ports.SerialPort(PortName, 115200, System.IO.Ports.Parity.None, 8, System.IO.Ports.StopBits.One);
			com.Open();

			int currentImage = 0;
			bool isPlaying = false;

			while (currentImage < images.Length)
			{
				com.ReadByte();

				if (!isPlaying)
				{
					player.controls.play();
					isPlaying = true;
				}

				var currentPosition = player.controls.currentPosition;

				currentImage = (int)(FPS * currentPosition);
				com.Write(images[Math.Min(currentImage, images.Length - 1)], 0, 504);
			}
			
			com.Close();
		}
	}
}
