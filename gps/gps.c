//
// Created by ocd on 2021/8/24.
//
#include "gps.h"

// 数据分割，可以分割两个连续的分隔符
static char* strsplit(char** stringp, const char* delim)
{
    char* start = *stringp;
    char* p;

    p = (start != NULL) ? strpbrk(start, delim) : NULL;

    if (p == NULL)
    {
        *stringp = NULL;
    }
    else
    {
        *p = '\0';
        *stringp = p + 1;
    }

    return start;
}

// 统计字符串在另一个字符串中出现的次数
static int strstr_cnt(char *str, char *substr)
{
    char *srcStr = str;
    int count = 0;

    do
    {
        srcStr = strstr(srcStr, substr);
        if(srcStr != NULL)
        {
            count++;
            srcStr = srcStr + strlen(substr);
        }
        else
        {
            break;
        }
    }while (*srcStr != '\0');

    return count;
}

#if ENABLE_GGA
static GGA gga_data_parse(char *gga_data)
{
    GGA gga;
    unsigned char times = 0;
    char *p;
    char *end;
    char *s = strdup(gga_data);

    p = strsplit(&s, ",");
    while (p)
    {
        switch (times)
        {
            case 1:   // UTC
                strcpy(gga.utc, p);
                break;
            case 2:   // lat
                gga.lat = strtod(p, NULL);
                break;
            case 3:   // lat dir
                gga.lat_dir = p[0];
                break;
            case 4:   // lon
                gga.lon = strtod(p, NULL);
                break;
            case 5:   // lon dir
                gga.lon_dir = p[0];
                break;
            case 6:   // quality
                gga.quality = (unsigned char)strtol(p, NULL, 10);
                break;
            case 7:   // sats
                gga.sats = (unsigned char)strtol(p, NULL, 10);
                break;
            case 8:   // hdop
                gga.hdop = (unsigned char)strtol(p, NULL, 10);
                break;
            case 9:   // alt
                gga.alt = strtof(p, NULL);
                break;
            case 11:  // undulation
                gga.undulation = strtof(p, NULL);
                break;
            case 13:  // age
                gga.age = (unsigned char)strtol(p, NULL, 10);
                break;
            case 14:  // stn_ID
                end = (char *)malloc(sizeof(p));
                strncpy(end, p, strlen(p)-3);
                end[strlen(p)-3] = '\0';
                gga.stn_ID = (unsigned short )strtol(end, NULL, 10);
                free(end);
                break;
            default:
                break;
        }
        p = strsplit(&s, ",");
        times++;
    }
    free(s);
    return gga;
}
#endif

#if ENABLE_GLL
static GLL gll_data_parse(char *gll_data)
{
    GLL gll;
    unsigned char times = 0;
    char *p;
    char *s = strdup(gll_data);

    p = strsplit(&s, ",");
    while (p)
    {
        switch (times)
        {
            case 1:   // lat
                gll.lat = strtod(p, NULL);
                break;
            case 2:   // lat dir
                gll.lat_dir = p[0];
                break;
            case 3:   // lon
                gll.lon = strtod(p, NULL);
                break;
            case 4:   // lon dir
                gll.lon_dir = p[0];
                break;
            case 5:   // lon dir
                strcpy(gll.utc, p);
                break;
            case 6:  // data status
                gll.data_status = p[0];
                break;
            default:
                break;
        }
        p = strsplit(&s, ",");
        times++;
    }
    free(s);
    return gll;
}
#endif

#if ENABLE_GSA
static GSA_PRN *get_prn_data(char *gps_data, char *prefix)
{
    GSA_PRN *gsa_prn;
    unsigned char times = 0;
    unsigned char i;
    unsigned char sentences_index = 0;  // 累计找到gsa字段的个数
    char *p;
    char *s;
    char *sentences;
    int gsa_count;

    gsa_count = strstr_cnt(gps_data, prefix);

    gsa_prn = (GSA_PRN *)malloc(sizeof(GSA_PRN) * (gsa_count * 12 + 1));
    memset(gsa_prn, 0, sizeof(GSA_PRN) * (gsa_count * 12 + 1));
    sentences = strtok(gps_data, "\r\n");
    while (sentences)
    {
        if (strstr(sentences, prefix))
        {
            sentences_index++;
            s = strdup(sentences);
            p = strsplit(&s, ",");
            while (p)
            {
                if (times == 2)
                {
                    for (i=0; i<12; i++)
                    {
                        p = strsplit(&s, ",");
                        (gsa_prn+i+(sentences_index-1)*12)->total = (unsigned char)gsa_count * 12;
                        (gsa_prn+i+(sentences_index-1)*12)->prn_ID = i + (sentences_index - 1) * 12;
                        (gsa_prn+i+(sentences_index-1)*12)->prn = (unsigned char)strtol(p, NULL, 10);
                    }
                }
                p = strsplit(&s, ",");
                times++;
            }
            times = 0;
        }
        sentences = strtok(NULL, "\r\n");
    }
    free(s);
    return gsa_prn;
}

static GSA gsa_data_parse(char *gsa_data, char *gpsdata, char *prefix)
{
    GSA gsa;
    unsigned char times = 0;
    char *p;
    char *end;
    char *s = strdup(gsa_data);
    char *alldata = strdup(gpsdata);

    p = strsplit(&s, ",");
    while (p)
    {
        switch (times)
        {
            case 1:   // mode_MA
                gsa.mode_MA = p[0];
                break;
            case 2:   // mode_123
                gsa.mode_123 = p[0];
                break;
            case 3:   // prn
                gsa.gsa_prn = get_prn_data(alldata,prefix);
                break;
            case 15:  // pdop
                gsa.pdop = strtod(p, NULL);
                break;
            case 16:  // hdop
                gsa.hdop = strtod(p, NULL);
                break;
            case 17:  // vdop
                end = (char *)malloc(sizeof(p));
                strncpy(end, p, strlen(p)-3);
                end[strlen(p)-3] = '\0';
                gsa.vdop = strtod(end, NULL);
                free(end);
            default:
                break;
        }
        p = strsplit(&s, ",");
        times++;
    }
    free(s);
    return gsa;
}
#endif

#if ENABLE_RMC
static RMC rmc_data_parse(char *rmc_data)
{
    RMC rmc;
    unsigned char times = 0;
    char *p;
    char *s = strdup(rmc_data);

    p = strsplit(&s, ",");
    while (p)
    {
        switch (times)
        {
            case 1:   // UTC
                strcpy(rmc.utc, p);
                break;
            case 2:   // pos status
                rmc.pos_status = p[0];
                break;
            case 3:   // lat
                rmc.lat = strtod(p, NULL);
                break;
            case 4:   // lat dir
                rmc.lat_dir = p[0];
                break;
            case 5:   // lon
                rmc.lon = strtod(p, NULL);
                break;
            case 6:   // lon dir
                rmc.lon_dir = p[0];
                break;
            case 7:   // speen Kn
                rmc.speed_Kn = strtod(p, NULL);
                break;
            case 8:   // track true
                rmc.track_true = strtod(p, NULL);
                break;
            case 9:   // date
                strcpy(rmc.date, p);
                break;
            case 10:  // mag var
                rmc.mag_var = strtod(p, NULL);
                break;
            case 11:  // var dir
                rmc.var_dir = p[0];
                break;
            case 14:  // mode ind
                rmc.mode_ind = p[0];
                break;
            default:
                break;
        }
        p = strsplit(&s, ",");
        times++;
    }
    free(s);
    return rmc;
}
#endif

#if ENABLE_VTG
static VTG vtg_data_parse(char *vtg_data)
{
    VTG vtg;
    unsigned char times = 0;
    char *p;
    char *s = strdup(vtg_data);

    p = strsplit(&s, ",");
    while (p)
    {
        switch (times)
        {
            case 1:   // track true
                vtg.track_true = strtod(p, NULL);
                break;
            case 3:   // track mag
                vtg.track_mag = strtod(p, NULL);
                break;
            case 5:   // speed Kn
                vtg.speed_Kn = strtod(p, NULL);
                break;
            case 7:   // speed Km
                vtg.speed_Km = strtod(p, NULL);
                break;
            default:
                break;
        }
        p = strsplit(&s, ",");
        times++;
    }
    free(s);
    return vtg;
}
#endif

#if ENABLE_GSV
static SAT_INFO *get_sats_info(char *gps_data, unsigned char sats, char *prefix)
{
    SAT_INFO *sats_info;
    unsigned char times = 0;
    unsigned char msgs = 0;
    unsigned char msg = 0;
    unsigned char for_times;
    unsigned char i;
    char *p;
    char *s;
    char *sentences;

    sats_info = (SAT_INFO *)malloc(sizeof(SAT_INFO) * (sats+1));
    memset(sats_info, 0, sizeof(SAT_INFO) * (sats+1));
    sentences = strtok(gps_data, "\r\n");
    while (sentences)
    {
        if (strstr(sentences, prefix))
        {
            s = strdup(sentences);
            p = strsplit(&s, ",");
            while (p)
            {
                switch (times)
                {
                    case 1:   // msgs
                        msgs = (unsigned char) strtol(p, NULL, 10);
                        break;
                    case 2:   // msg
                        msg = (unsigned char) strtol(p, NULL, 10);
                        break;
                    case 3:   // sat info
                        for_times = (msgs == msg) ? ((sats % 4) ? sats % 4 : 4) : 4;
                        for (i = 0; i < for_times; i++)
                        {
                            p = strsplit(&s, ",");
                            (sats_info+(msg-1)*4+i)->prn = (unsigned char) strtol(p, NULL, 10);
                            p = strsplit(&s, ",");
                            (sats_info+(msg-1)*4+i)->elev = (unsigned char) strtol(p, NULL, 10);
                            p = strsplit(&s, ",");
                            (sats_info+(msg-1)*4+i)->azimuth = (unsigned short) strtol(p, NULL, 10);
                            p = strsplit(&s, ",");
                            (sats_info+(msg-1)*4+i)->SNR = (unsigned char) strtol(p, NULL, 10);
                        }
                        break;
                    default:
                        break;
                }
                p = strsplit(&s, ",");
                times++;
            }
            times = 0;
        }
        sentences = strtok(NULL, "\r\n");
    }
    free(s);
    return sats_info;
}

static GSV gsv_data_parse(char *gsv_data, char *gps_data, char *prefix)
{
    GSV gsv;
    unsigned char times = 0;
    char *p;
    char *s = strdup(gsv_data);
    char *src_data = strdup(gps_data);

    p = strsplit(&s, ",");
    while (p)
    {
        switch (times)
        {
            case 1:   // msgs
                gsv.msgs = (unsigned char)strtol(p, NULL, 10);
                break;
            case 2:   // msg
                gsv.msg = (unsigned char)strtol(p, NULL, 10);
                break;
            case 3:   // sats
                gsv.sats = (unsigned char)strtol(p, NULL, 10);
                gsv.sat_info = get_sats_info(src_data, gsv.sats, prefix);
                break;
            default:
                break;
        }
        p = strsplit(&s, ",");
        times++;
    }
    free(s);
    return gsv;
}
#endif

#if ENABLE_UTC
static UTC utc_parse(char *date, char *time)
{
    UTC utc_data;
    unsigned int date_int;
    double time_float;

    date_int = (unsigned int)strtol(date, NULL, 10);
    utc_data.DD = date_int / 10000;
    utc_data.MM = date_int % 10000 / 100;
    utc_data.YY = date_int % 100;
    time_float = strtod(time, NULL);
    utc_data.hh = (unsigned int)time_float / 10000;
    utc_data.mm = (unsigned int)time_float % 10000 / 100;
    utc_data.ss = (unsigned int)time_float % 100;
    utc_data.ds = (unsigned short)(time_float - (unsigned int)time_float);

    return utc_data;
}
#endif

GPS gps_data_parse(char* gps_src)
{
    GPS gps_all;
    char *str_buffer = strdup(gps_src);

#if ENABLE_GGA
    GGA default_gga_data = {"\0",0.0,'N',0.0,'S',0,0,0,0,0,0,0};
    gps_src = strdup(str_buffer);
    gps_all.gga_data = strstr(gps_src, PRE_GGA) ? gga_data_parse(strtok(strstr(gps_src, PRE_GGA), "\r\n")) : default_gga_data;
#endif

#if ENABLE_GLL
    GLL default_gll_data = {0.0,'\0',0.0,'\0',"\0",'\0'};
    gps_src = strdup(str_buffer);
    gps_all.gll_data = strstr(gps_src, PRE_GLL) ? gll_data_parse(strtok(strstr(gps_src, PRE_GLL), "\r\n")) : default_gll_data;
#endif

#if ENABLE_GSA
    GSA_PRN default_gsa_prn_data = {0,0,0};
    GSA default_gsa_data = {'\0','\0',0.0,0.0,0.0,&default_gsa_prn_data};

	gps_src = strdup(str_buffer);
    gps_all.gpgsa_data = strstr(gps_src, PRE_GPGSA) ? gsa_data_parse(strtok(strstr(gps_src, PRE_GPGSA), "\r\n"), str_buffer, PRE_GPGSA) : default_gsa_data;

	gps_src = strdup(str_buffer);
    gps_all.bdgsa_data = strstr(gps_src, PRE_BDGSA) ? gsa_data_parse(strtok(strstr(gps_src, PRE_BDGSA), "\r\n"), str_buffer, PRE_BDGSA) : default_gsa_data;
#endif

#if ENABLE_RMC
    RMC default_rmc_data = {"\0",'\0',0.0,'\0',0.0,'\0',0.0,0.0,"\0",0.0,'\0','\0'};
    gps_src = strdup(str_buffer);
    gps_all.rmc_data = strstr(gps_src, PRE_RMC) ? rmc_data_parse(strtok(strstr(gps_src, PRE_RMC), "\r\n")) : default_rmc_data;
#endif

#if ENABLE_VTG
    VTG default_vtg_data = {0.0,0.0,0.0,0.0};
    gps_src = strdup(str_buffer);
    gps_all.vtg_data = strstr(gps_src, PRE_VTG) ? vtg_data_parse(strtok(strstr(gps_src, PRE_VTG), "\r\n")) : default_vtg_data;
#endif

#if ENABLE_GSV
    SAT_INFO default_sat_info_data = {0,0,0,0};
    GSV default_gsv_data = {0,0,0,&default_sat_info_data};

    gps_src = strdup(str_buffer);
    gps_all.gpgsv_data = strstr(gps_src, PRE_GPGSV) ? gsv_data_parse(strtok(strstr(gps_src, PRE_GPGSV), "\r\n"), str_buffer, PRE_GPGSV) : default_gsv_data;

    gps_src = strdup(str_buffer);
    gps_all.bdgsv_data = strstr(gps_src, PRE_BDGSV) ? gsv_data_parse(strtok(strstr(gps_src, PRE_BDGSV), "\r\n"), str_buffer, PRE_BDGSV) : default_gsv_data;
#endif

#if ENABLE_UTC && ENABLE_RMC
    gps_all.utc = utc_parse(gps_all.rmc_data.date, gps_all.rmc_data.utc);
#endif

    free(str_buffer);
    free(gps_src);
    return gps_all;
}

//test data
int main()
{
    GPS gps;
    unsigned char i;
    char gps_data[] = "$GNRMC,013300.00,A,2240.84105,N,11402.70763,E,0.007,,220319,,,D*69\r\n"
                      "$GNVTG,,T,,M,0.007,N,0.014,K,D*3A\r\n"
                      "$GNGGA,013300.00,2240.84105,N,11402.70763,E,2,12,0.59,70.5,M,-2.5,M,,0000*68\r\n"
                      "$GPGSA,A,3,01,20,19,13,,,,,,,,,40.4,24.4,32.2*0A\r\n"
                      "$BDGSA,A,3,07,08,13,14,,,,,,,,,2.68,1.65,2.11*17\r\n"
                      "$GPGSV,4,1,16,01,00,300,,10,56,178,51,12,12,038,38,14,47,345,48*79\r\n"
                      "$BDGSV,1,1,04,07,76,089,39,08,78,285,36,13,56,236,36,14,16,073,34*6E\r\n"
                      "$GNGLL,2240.84105,N,11402.70763,E,013300.00,A,D*7C\r\n";
    gps = gps_data_parse(gps_data);

#if ENABLE_RMC
		printf("----------RMC DATA----------\n");
		printf("utc:%s\n", gps.rmc_data.utc);
		printf("lat:%f\n", gps.rmc_data.lat);
		printf("lat_dir:%c\n", gps.rmc_data.lat_dir);
		printf("lon:%f\n", gps.rmc_data.lon);
		printf("lon_dir:%c\n", gps.rmc_data.lon_dir);
		printf("speed_Kn:%f\n", gps.rmc_data.speed_Kn);
		printf("track_true:%f\n", gps.rmc_data.track_true);
		printf("date:%s\n", gps.rmc_data.date);
		printf("mag_var:%f\n", gps.rmc_data.mag_var);
		printf("var_dir:%c\n", gps.rmc_data.var_dir);
		printf("mode_ind:%c\n", gps.rmc_data.mode_ind);
#endif

#if ENABLE_VTG
		printf("----------VTG DATA----------\n");
		printf("track_true:%f\n", gps.vtg_data.track_true);
		printf("track_mag:%f\n", gps.vtg_data.track_mag);
		printf("speen_Kn:%f\n", gps.vtg_data.speed_Kn);
		printf("speed_Km:%f\n", gps.vtg_data.speed_Km);
#endif

#if ENABLE_GGA
    printf("----------GGA DATA----------\n");
    printf("utc:%s\n", gps.gga_data.utc);
    printf("lat:%f\n", gps.gga_data.lat);
    printf("lat_dir:%c\n", gps.gga_data.lat_dir);
    printf("lon:%f\n", gps.gga_data.lon);
    printf("lon_dir:%c\n", gps.gga_data.lon_dir);
    printf("quality:%d\n", gps.gga_data.quality);
    printf("sats:%d\n", gps.gga_data.sats);
    printf("hdop:%f\n", gps.gga_data.hdop);
    printf("alt:%f\n", gps.gga_data.alt);
    printf("undulation:%f\n", gps.gga_data.undulation);
    printf("age:%d\n", gps.gga_data.age);
    printf("stn_ID:%d\n", gps.gga_data.stn_ID);
#endif

#if ENABLE_GSA
		printf("----------GPGSA DATA----------\n");
		printf("mode_MA:%c\n", gps.gpgsa_data.mode_MA);
		printf("mode_123:%c\n", gps.gpgsa_data.mode_123);
		printf("total:%d\n", gps.gpgsa_data.gsa_prn[0].total);
		for (i=0; i<gps.gpgsa_data.gsa_prn[0].total; i++)
		{
			printf("prn%d:%d\n", (i+1), gps.gpgsa_data.gsa_prn[i].prn);
		}
		printf("pdop:%f\n", gps.gpgsa_data.pdop);
		printf("hdop:%f\n", gps.gpgsa_data.hdop);
		printf("vdop:%f\n", gps.gpgsa_data.vdop);
		// gps.gpgsa_data.gsa_prn是动态分配的内存，用完记得释放,否则会造成内存泄漏
		free(gps.gpgsa_data.gsa_prn);
	
		printf("----------BDGSA DATA----------\n");
		printf("mode_MA:%c\n", gps.bdgsa_data.mode_MA);
		printf("mode_123:%c\n", gps.bdgsa_data.mode_123);
		printf("total:%d\n", gps.bdgsa_data.gsa_prn[0].total);
		for (i=0; i<gps.bdgsa_data.gsa_prn[0].total; i++)
		{
			printf("prn%d:%d\n", (i+1), gps.bdgsa_data.gsa_prn[i].prn);
		}
		printf("pdop:%f\n", gps.bdgsa_data.pdop);
		printf("hdop:%f\n", gps.bdgsa_data.hdop);
		printf("vdop:%f\n", gps.bdgsa_data.vdop);
		// gps.bdgsa_data.gsa_prn是动态分配的内存，用完记得释放,否则会造成内存泄漏
		free(gps.bdgsa_data.gsa_prn);
#endif

#if ENABLE_GSV
    printf("----------GPGSV DATA----------\n");
    printf("msgs:%d\n", gps.gpgsv_data.msgs);
    printf("msg:%d\n", gps.gpgsv_data.msg);
    printf("sats:%d\n", gps.gpgsv_data.sats);
    for (i=0;i<gps.gpgsv_data.sats; i++)
    {
        printf("prn%d:%d\n", i+1, gps.gpgsv_data.sat_info[i].prn);
        printf("evel%d:%d\n", i+1, gps.gpgsv_data.sat_info[i].elev);
        printf("azimuth%d:%d\n", i+1, gps.gpgsv_data.sat_info[i].azimuth);
        printf("SNR%d:%d\n", i+1, gps.gpgsv_data.sat_info[i].SNR);
    }
    // 用完释放gps.gpgsv_data.sat_info内存
    if (gps.gpgsv_data.sats) free(gps.gpgsv_data.sat_info);

    printf("----------BDGSV DATA----------\n");
    printf("msgs:%d\n", gps.bdgsv_data.msgs);
    printf("msg:%d\n", gps.bdgsv_data.msg);
    printf("sats:%d\n", gps.bdgsv_data.sats);
    for (i=0; i<gps.bdgsv_data.sats; i++)
    {
        printf("prn%d:%d\n", i+1, gps.bdgsv_data.sat_info[i].prn);
        printf("evel%d:%d\n", i+1, gps.bdgsv_data.sat_info[i].elev);
        printf("azimuth%d:%d\n", i+1, gps.bdgsv_data.sat_info[i].azimuth);
        printf("SNR%d:%d\n", i+1, gps.bdgsv_data.sat_info[i].SNR);
    }
    if (gps.bdgsv_data.sats) free(gps.bdgsv_data.sat_info);
#endif

#if ENABLE_GLL
		printf("----------GLL DATA----------\n");
		printf("utc:%s\n", gps.gll_data.utc);
		printf("lat:%f\n", gps.gll_data.lat);
		printf("lat_dir:%c\n", gps.gll_data.lat_dir);
		printf("lon:%f\n", gps.gll_data.lon);
		printf("lon_dir:%c\n", gps.gll_data.lon_dir);
		printf("data_status:%c\n", gps.gll_data.data_status);
#endif

#if ENABLE_UTC && ENABLE_RMC
    printf("----------UTC DATA----------\n");
    printf("year:20%02d\n", gps.utc.YY);
    printf("month:%02d\n", gps.utc.MM);
    printf("date:%02d\n", gps.utc.DD);
    printf("hour:%02d\n", gps.utc.hh);
    printf("minutes:%02d\n", gps.utc.mm);
    printf("second:%02d\n", gps.utc.ss);
    printf("ds:%02d\n", gps.utc.ds);
#endif

    return 0;
}

