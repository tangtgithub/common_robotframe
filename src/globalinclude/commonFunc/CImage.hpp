#ifndef _CIMAGE_H_
#define _CIMAGE_H_

#include <iostream>
#include <fstream>
#include <string>


#define MAKEUS(a, b)    (static_cast<unsigned short> ( (static_cast<unsigned short>(a))<<8 | (static_cast<unsigned short>(b)) ))
#define MAKEUI(a,b,c,d) (static_cast<unsigned int>  ( (static_cast<unsigned int> (a)) << 24 | (static_cast<unsigned int> (b)) << 16 | (static_cast<unsigned int> (c)) << 8 | (static_cast<unsigned int> (d)) ))

#define M_DATA  0x00
#define M_SOF0  0xc0
#define M_DHT   0xc4
#define M_SOI   0xd8
#define M_EOI   0xd9
#define M_SOS   0xda
#define M_DQT   0xdb
#define M_DNL   0xdc
#define M_DRI   0xdd
#define M_APP0  0xe0
#define M_APPF  0xef
#define M_COM   0xfe




class CImage
{
private:
	long  m_Width;
	long  m_Height;

	int get_extension(std::string fname)
    {
        char c = fname.at(fname.length() - 1);
        char c2 = fname.at(fname.length() - 3);

        if ((c == 'f') && (c2 == 'g')) {  // file extension name is gif 
            return 1;
        }
        else if ((c == 'g') && (c2 == 'j')) { // file extension name is jpg
            return 2;
        }
        else if ((c == 'g') && (c2 == 'p')) { // file extension name is png
            return 3;
        }
        else if ((c == 'p') && (c2 == 'b')) { // file extension name is bmp
            return 4;
        }
        return 0;
    }

	int GetJPEGWidthHeight(const char* path, unsigned int *punWidth, unsigned int *punHeight)
    {
        
        int Finished = 0;
        unsigned char id, ucHigh, ucLow;
        FILE *pfRead;

        *punWidth = 0;
        *punHeight = 0;

        //if (fopen_s(&pfRead, path, "rb") != 0)
        pfRead = fopen(path, "rb");
        if (pfRead == NULL)
        {
            printf("[GetJPEGWidthHeight]:can't open file:%s\n", path);
            return -1;
        }
        size_t res;

        while (!Finished)
        {
            if (!fread(&id, sizeof(char), 1, pfRead) || id != 0xff || !fread(&id, sizeof(char), 1, pfRead))
            {
                Finished = -2;
                break;
            }

            if (id >= M_APP0 && id <= M_APPF)
            {
                res = fread(&ucHigh, sizeof(char), 1, pfRead);
                res = fread(&ucLow, sizeof(char), 1, pfRead);
                res = fseek(pfRead, static_cast<long>(MAKEUS(ucHigh, ucLow) - 2), SEEK_CUR);
                continue;
            }

            switch (id)
            {
                case M_SOI:
                    break;

                case M_COM:
                case M_DQT:
                case M_DHT:
                case M_DNL:
                case M_DRI:
                    res = fread(&ucHigh, sizeof(char), 1, pfRead);
                    res = fread(&ucLow, sizeof(char), 1, pfRead);
                    res = fseek(pfRead, static_cast<long>(MAKEUS(ucHigh, ucLow) - 2), SEEK_CUR);
                    break;

                case M_SOF0:
                    res = fseek(pfRead, 3L, SEEK_CUR);
                    res = fread(&ucHigh, sizeof(char), 1, pfRead);
                    res = fread(&ucLow, sizeof(char), 1, pfRead);
                    *punHeight = static_cast<unsigned int>(MAKEUS(ucHigh, ucLow));
                    res = fread(&ucHigh, sizeof(char), 1, pfRead);
                    res = fread(&ucLow, sizeof(char), 1, pfRead);
                    *punWidth = static_cast<unsigned int>(MAKEUS(ucHigh, ucLow));
                    return 0;

                case M_SOS:
                case M_EOI:
                case M_DATA:
                    Finished = -1;
                    break;

                default:
                    res = fread(&ucHigh, sizeof(char), 1, pfRead);
                    res = fread(&ucLow, sizeof(char), 1, pfRead);
                    printf("[GetJPEGWidthHeight]:unknown id: 0x%x ;res=%lu ; length=%hd\n", id, res, MAKEUS(ucHigh, ucLow));
                    if (fseek(pfRead, static_cast<long>(MAKEUS(ucHigh, ucLow) - 2), SEEK_CUR) != 0)
                        Finished = -2;
                    break;
            }
        }

        if (Finished == -1)
            printf("[GetJPEGWidthHeight]:can't find SOF0!\n");
        else if (Finished == -2)
            printf("[GetJPEGWidthHeight]:jpeg format error!\n");
        fclose(pfRead);
        return -1;
    }

	int GetPNGWidthHeight(const char* path, unsigned int* punWidth, unsigned int* punHeight)
    {
        
        unsigned char uc[4];
        FILE *pfRead;

        *punWidth = 0;
        *punHeight = 0;

        //if (fopen_s(&pfRead, path, "rb") != 0)
        pfRead = fopen(path, "rb");
        if (pfRead == NULL)
        {
            printf("[GetPNGWidthHeight]:can't open file:%s\n", path);
            return -1;
        }
        size_t res;
        for (int i = 0; i < 4; i++)
            res = fread(&uc[i], sizeof(unsigned char), 1, pfRead);
        if (MAKEUI(uc[0], uc[1], uc[2], uc[3]) != 0x89504e47)
            printf("[GetPNGWidthHeight]:png = %s format error \n", path);
        for (int i = 0; i < 4; i++)
            res = fread(&uc[i], sizeof(unsigned char), 1, pfRead);
        if (MAKEUI(uc[0], uc[1], uc[2], uc[3]) != 0x0d0a1a0a)
            printf("[GetPNGWidthHeight]:png = %s res = %lu format error\n", path, res);

        fseek(pfRead, 16, SEEK_SET);
        for (int i = 0; i < 4; i++)
            res = fread(&uc[i], sizeof(unsigned char), 1, pfRead);
        *punWidth = MAKEUI(uc[0], uc[1], uc[2], uc[3]);
        for (int i = 0; i < 4; i++)
            res = fread(&uc[i], sizeof(unsigned char), 1, pfRead);
        *punHeight = MAKEUI(uc[0], uc[1], uc[2], uc[3]);

        fclose(pfRead);
        return 0;
    }

public:
	CImage()
	{
		m_Width = 0;
		m_Height = 0;

	}
	void LoadImage(char* fname)
    {
        
        m_Width = m_Height = 0;

        ifstream ffin(fname, std::ios::binary);

        if (!ffin) {
            cout << "Can not open this file." << endl;
            return;
        }
        int result = get_extension(fname);
        char s1[2] = { 0 }, s2[2] = { 0 };

        switch (result)
        {
            case 1:  // gif  
                ffin.seekg(6);
                ffin.read(s1, 2);
                ffin.read(s2, 2);
                m_Width = static_cast<unsigned int>(s1[1]) << 8 | static_cast<unsigned int>(s1[0]);
                m_Height = static_cast<unsigned int>(s2[1]) << 8 | static_cast<unsigned int>(s2[0]);
                break;
            case 2:  // jpg
                ffin.seekg(164);
                ffin.read(s1, 2);
                ffin.read(s2, 2);
                m_Width = static_cast<unsigned int>(s1[1]) << 8 | static_cast<unsigned int>(s1[0]);
                m_Height = static_cast<unsigned int>(s2[1]) << 8 | static_cast<unsigned int>(s2[0]);
                break;
            case 3:   // png
                ffin.seekg(17);
                ffin.read(s1, 2);
                ffin.seekg(2, std::ios::cur);
                ffin.read(s2, 2);
                m_Width = static_cast<unsigned int>(s1[1]) << 8 | static_cast<unsigned int>(s1[0]);
                m_Height = static_cast<unsigned int>(s2[1]) << 8 | static_cast<unsigned int>(s2[0]);
                spdlog::info("nav_recv image ----   m_Width = {},m_Height = {}", m_Width, m_Height);
                break;
            case 4:   // bmp    
                ffin.seekg(18);
                ffin.read(s1, 2);
                ffin.seekg(2, std::ios::cur);
                ffin.read(s2, 2);
                m_Width = static_cast<unsigned int>(s1[1]) << 8 | static_cast<unsigned int>(s1[0]);
                m_Height = static_cast<unsigned int>(s2[1]) << 8 | static_cast<unsigned int>(s2[0]);
                break;
            default:
                cout << "NO" << endl;
                break;
        }
        ffin.close();
    }

	long get_width()
	{
		return m_Width;
	}

	long get_height()
	{
		return m_Height;
	}


	void GetPicWidthHeight(const char* path, unsigned int *punWidth, unsigned int *punHeight)
    {
        
        size_t len = strlen(path);
        if (len <= 4)
            printf("[GetPicWidthHeight]:picture name is too short\n");
        if (!strncmp(path + len - 3, "jpg", 3))
            GetJPEGWidthHeight(path, punWidth, punHeight);
        else if (!strncmp(path + len - 3, "png", 3))
            GetPNGWidthHeight(path, punWidth, punHeight);
        else
            printf("[GetPicWidthHeight]:only support jpg and png\n");
    }

};













#endif 
