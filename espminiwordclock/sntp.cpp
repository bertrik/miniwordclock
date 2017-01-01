
int sntp_sync(int localPort, IPAddress& address, int timeout, uint32_t *secsSince2000)
{
    // prepare NTP packet
    uint8_t buf[NTP_PACKET_SIZE];
    memset(buf, 0, sizeof(buf));
    buf[0] = 0b11100011;   // LI, Version, Mode
    buf[1] = 0;     // Stratum, or type of clock
    buf[2] = 6;     // Polling Interval
    buf[3] = 0xEC;  // Peer Clock Precision
    // 8 bytes of zero for Root Delay & Root Dispersion

    // send it
    print("sending NTP packet...\n");
    WiFiUDP udp;
    udp.begin(localPort);
    udp.beginPacket(address, 123); //NTP requests are to port 123
    udp.write(buf, sizeof(buf));
    udp.endPacket();
    
    // wait for response
    print("waiting for response...");
    int cb;
    unsigned long start = millis();
    while ((cb = udp.parsePacket()) <= 0) {
        if ((millis() - start) > timeout) {
            print("timeout!\n");
            return -1;
        }
        delay(10);
    }
    print("got %d bytes\n", cb);

    // decode response
    udp.read(buf, sizeof(buf));
    unsigned long highWord = word(buf[40], buf[41]);
    unsigned long lowWord = word(buf[42], buf[43]);
    unsigned long secsSince1900 = highWord << 16 | lowWord;
    // convert to seconds since 2000-1-1 00:00:00
    *secsSince2000 = secsSince1900 - 3155673600;

    return 0;
}

