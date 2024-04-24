//******************************* define eeprom ********************//
u16 passRoot_addr 		=0x004;
u16 user1_addr	 		=0x010;
u16 pass1_addr	 		=0x01c;
u16 user2_addr	 		=0x028;
u16 pass2_addr	 		=0x033;


//phan tren giu nguyen//
//
u16 port_number_addr 		=0x100;
u16 entry_number_addr 		=0x108;

u16 active_time_hi_addr 			=0x110;
u16 active_time_lo_addr			=0x111;
u16 inactive_time_hi_addr 		=0x114;
u16 inactive_time_lo_addr		=0x115;
u16 timestamp_1_addr				=0x118;
u16 timestamp_2_addr				=0x119;
u16 timestamp_3_addr				=0x11a;
u16 timestamp_4_addr				=0x11b;


// Mirror old
u16  	port_mirror_1_hi_addr		=0x120;
u16		port_mirror_1_lo_addr		=0x121;
u16 	port_mirror_2_hi_addr 	    =0x122;
u16		port_mirror_2_lo_addr		=0x123;
u16 	port_mirror_3_hi_addr		=0x124;
u16 	port_mirror_3_lo_addr		=0x125;
u16		port_mirror_4_hi_addr		=0x126;
u16		port_mirror_4_lo_addr		=0x127;
u16		port_mirror_5_hi_addr		=0x128;
u16		port_mirror_5_lo_addr		=0x129;
u16		port_mirror_6_hi_addr		=0x130;
u16		port_mirror_6_lo_addr		=0x131;
u16		port_mirror_7_hi_addr		=0x132;
u16		port_mirror_7_lo_addr		=0x133;
u16		port_mirror_8_hi_addr		=0x134;
u16		port_mirror_8_lo_addr		=0x135;
u16 	mirror_status_addr			=0x13a;

//Mirror new
u16 ingress_port1_eeprom_lo_addr = 0x120;
u16 ingress_port1_eeprom_hi_addr = 0x121;
u16 ingress_port2_eeprom_lo_addr = 0x122;
u16 ingress_port2_eeprom_hi_addr = 0x123;
u16 ingress_port3_eeprom_lo_addr = 0x124;
u16 ingress_port3_eeprom_hi_addr = 0x125;
u16 ingress_port4_eeprom_lo_addr = 0x126;
u16 ingress_port4_eeprom_hi_addr = 0x127;
u16 ingress_port5_eeprom_lo_addr = 0x128;
u16 ingress_port5_eeprom_hi_addr = 0x129;
u16 ingress_port6_eeprom_lo_addr = 0x12a;
u16 ingress_port6_eeprom_hi_addr = 0x12b;
u16 ingress_port7_eeprom_lo_addr = 0x12c;
u16 ingress_port7_eeprom_hi_addr = 0x12d;
u16 ingress_port8_eeprom_lo_addr = 0x12e;
u16 ingress_port8_eeprom_hi_addr = 0x12f;
u16 ingress_port9_eeprom_lo_addr = 0x130;
u16 ingress_port9_eeprom_hi_addr = 0x131;
u16 ingress_port10_eeprom_lo_addr = 0x132;
u16 ingress_port10_eeprom_hi_addr = 0x133;
u16 ingress_port11_eeprom_lo_addr = 0x134;
u16 ingress_port11_eeprom_hi_addr = 0x135;
u16 ingress_port12_eeprom_lo_addr = 0x136;
u16 ingress_port12_eeprom_hi_addr = 0x137;

u16 egress_port1_eeprom_lo_addr = 0x138;
u16 egress_port1_eeprom_hi_addr = 0x139;
u16 egress_port2_eeprom_lo_addr = 0x13a;
u16 egress_port2_eeprom_hi_addr = 0x13b;
u16 egress_port3_eeprom_lo_addr = 0x13c;
u16 egress_port3_eeprom_hi_addr = 0x13d;
u16 egress_port4_eeprom_lo_addr = 0x13e;
u16 egress_port4_eeprom_hi_addr = 0x13f;
u16 egress_port5_eeprom_lo_addr = 0x140;
u16 egress_port5_eeprom_hi_addr = 0x141;
u16 egress_port6_eeprom_lo_addr = 0x142;
u16 egress_port6_eeprom_hi_addr = 0x143;
u16 egress_port7_eeprom_lo_addr = 0x144;
u16 egress_port7_eeprom_hi_addr = 0x145;
u16 egress_port8_eeprom_lo_addr = 0x146;
u16 egress_port8_eeprom_hi_addr = 0x147;
u16 egress_port9_eeprom_lo_addr = 0x148;
u16 egress_port9_eeprom_hi_addr = 0x149;
u16 egress_port10_eeprom_lo_addr = 0x14a;
u16 egress_port10_eeprom_hi_addr = 0x14b;
u16 egress_port11_eeprom_lo_addr = 0x14c;
u16 egress_port11_eeprom_hi_addr = 0x14d;
u16 egress_port12_eeprom_lo_addr = 0x14e;
u16 egress_port12_eeprom_hi_addr = 0x14f;


// Vlan New

u16 	vlan_id_1_hi_addr			=0x200;
u16 	vlan_id_1_lo_addr			=0x201;
u16 	vlan_id_2_hi_addr			=0x202;
u16 	vlan_id_2_lo_addr			=0x203;
u16 	vlan_id_3_hi_addr			=0x204;
u16 	vlan_id_3_lo_addr			=0x205;
u16 	vlan_id_4_hi_addr			=0x206;
u16 	vlan_id_4_lo_addr			=0x207;
u16 	vlan_id_5_hi_addr			=0x208;
u16 	vlan_id_5_lo_addr			=0x209;
u16 	vlan_id_6_hi_addr			=0x20a;
u16 	vlan_id_6_lo_addr			=0x20b;
u16 	vlan_id_7_hi_addr			=0x20c;
u16 	vlan_id_7_lo_addr			=0x20d;
u16 	vlan_id_8_hi_addr			=0x20e;
u16 	vlan_id_8_lo_addr			=0x20f;
u16 	vlan_id_9_hi_addr			=0x210;
u16 	vlan_id_9_lo_addr			=0x211;
u16 	vlan_id_10_hi_addr			=0x212;
u16 	vlan_id_10_lo_addr			=0x213;
u16 	vlan_id_11_hi_addr			=0x214;
u16 	vlan_id_11_lo_addr			=0x215;
u16 	vlan_id_12_hi_addr			=0x216;
u16 	vlan_id_12_lo_addr			=0x217;
u16 	vlan_id_13_hi_addr			=0x218;
u16 	vlan_id_13_lo_addr			=0x219;
u16 	vlan_id_14_hi_addr			=0x21a;
u16 	vlan_id_14_lo_addr			=0x21b;
u16 	vlan_id_15_hi_addr			=0x21c;
u16 	vlan_id_15_lo_addr			=0x21d;
u16 	vlan_id_16_hi_addr			=0x21e;
u16 	vlan_id_16_lo_addr			=0x21f;

u16 	vlan_id_1_dst_port_lo_addr		=0x220;
u16 	vlan_id_1_dst_port_hi_addr		=0x221;
u16 	vlan_id_1_flag_lo_addr			=0x222;
u16 	vlan_id_1_flag_hi_addr			=0x223;
u16 	vlan_id_2_dst_port_lo_addr		=0x224;
u16 	vlan_id_2_dst_port_hi_addr		=0x225;
u16 	vlan_id_2_flag_lo_addr			=0x226;
u16 	vlan_id_2_flag_hi_addr			=0x227;
u16 	vlan_id_3_dst_port_lo_addr		=0x228;
u16 	vlan_id_3_dst_port_hi_addr		=0x229;
u16 	vlan_id_3_flag_lo_addr			=0x22a;
u16 	vlan_id_3_flag_hi_addr			=0x22b;
u16 	vlan_id_4_dst_port_lo_addr		=0x22c;
u16 	vlan_id_4_dst_port_hi_addr		=0x22d;
u16 	vlan_id_4_flag_lo_addr			=0x22e;
u16 	vlan_id_4_flag_hi_addr			=0x22f;
u16 	vlan_id_5_dst_port_lo_addr		=0x230;
u16 	vlan_id_5_dst_port_hi_addr		=0x231;
u16 	vlan_id_5_flag_lo_addr			=0x232;
u16 	vlan_id_5_flag_hi_addr			=0x233;
u16 	vlan_id_6_dst_port_lo_addr		=0x234;
u16 	vlan_id_6_dst_port_hi_addr		=0x235;
u16 	vlan_id_6_flag_lo_addr			=0x236;
u16 	vlan_id_6_flag_hi_addr			=0x237;
u16 	vlan_id_7_dst_port_lo_addr		=0x238;
u16 	vlan_id_7_dst_port_hi_addr		=0x239;
u16 	vlan_id_7_flag_lo_addr			=0x23a;
u16 	vlan_id_7_flag_hi_addr			=0x23b;
u16 	vlan_id_8_dst_port_lo_addr		=0x23c;
u16 	vlan_id_8_dst_port_hi_addr		=0x23d;
u16 	vlan_id_8_flag_lo_addr			=0x23e;
u16 	vlan_id_8_flag_hi_addr			=0x23f;
u16 	vlan_id_9_dst_port_lo_addr		=0x240;
u16 	vlan_id_9_dst_port_hi_addr		=0x241;
u16 	vlan_id_9_flag_lo_addr			=0x242;
u16 	vlan_id_9_flag_hi_addr			=0x243;
u16 	vlan_id_10_dst_port_lo_addr		=0x244;
u16 	vlan_id_10_dst_port_hi_addr		=0x245;
u16 	vlan_id_10_flag_lo_addr			=0x246;
u16 	vlan_id_10_flag_hi_addr			=0x247;
u16 	vlan_id_11_dst_port_lo_addr		=0x248;
u16 	vlan_id_11_dst_port_hi_addr		=0x249;
u16 	vlan_id_11_flag_lo_addr			=0x24a;
u16 	vlan_id_11_flag_hi_addr			=0x24b;
u16 	vlan_id_12_dst_port_lo_addr		=0x24c;
u16 	vlan_id_12_dst_port_hi_addr		=0x24d;
u16 	vlan_id_12_flag_lo_addr			=0x24e;
u16 	vlan_id_12_flag_hi_addr			=0x24f;
u16 	vlan_id_13_dst_port_lo_addr		=0x250;
u16 	vlan_id_13_dst_port_hi_addr		=0x251;
u16 	vlan_id_13_flag_lo_addr			=0x252;
u16 	vlan_id_13_flag_hi_addr			=0x253;
u16 	vlan_id_14_dst_port_lo_addr		=0x254;
u16 	vlan_id_14_dst_port_hi_addr		=0x255;
u16 	vlan_id_14_flag_lo_addr			=0x256;
u16 	vlan_id_14_flag_hi_addr			=0x257;
u16 	vlan_id_15_dst_port_lo_addr		=0x258;
u16 	vlan_id_15_dst_port_hi_addr		=0x259;
u16 	vlan_id_15_flag_lo_addr			=0x25a;
u16 	vlan_id_15_flag_hi_addr			=0x25b;
u16 	vlan_id_16_dst_port_lo_addr		=0x25c;
u16 	vlan_id_16_dst_port_hi_addr		=0x25d;
u16 	vlan_id_16_flag_lo_addr			=0x25e;
u16 	vlan_id_16_flag_hi_addr			=0x25f;

u16		vlan_status_eprom_addr				=0x300;
u16		mirror_status_eprom_addr			=0x301;


// Old
u16 	vlan_id_hi_addr[32]					={0x200, 0x202, 0x204, 0x206, 0x208, 0x20a, 0x20c, 0x20e, 0x210, 0x212, 0x214, 0x216, 0x218, 0x21a, 0x21c, 0x21e,
										  	  0x220, 0x222, 0x224, 0x226, 0x228, 0x22a, 0x22c, 0x22e, 0x230, 0x232, 0x234, 0x236, 0x238, 0x23a, 0x23c, 0x23e};
u16 	vlan_id_lo_addr[32]					={0x201, 0x203, 0x205, 0x207, 0x209, 0x20b, 0x20d, 0x20f, 0x211, 0x213, 0x215, 0x217, 0x219, 0x21b, 0x21d, 0x21f,
										  	  0x221, 0x223, 0x225, 0x227, 0x229, 0x22b, 0x22d, 0x22f, 0x231, 0x233, 0x235, 0x237, 0x239, 0x23b, 0x23d, 0x23f};
u16 	dest_port_vlan_hi_addr[32]			={0x300, 0x302, 0x304, 0x306, 0x308, 0x30a, 0x30c, 0x30e, 0x310, 0x312, 0x314, 0x316, 0x318, 0x31a, 0x31c, 0x31e,
	  	  	  	  	  	  	  	  	  	  	  0x320, 0x322, 0x324, 0x326, 0x328, 0x32a, 0x32c, 0x32e, 0x330, 0x332, 0x334, 0x336, 0x338, 0x33a, 0x33c, 0x33e};
u16		dest_port_vlan_lo_addr[32]			={0x301, 0x303, 0x305, 0x307, 0x309, 0x30b, 0x30d, 0x30f, 0x311, 0x313, 0x315, 0x317, 0x319, 0x31b, 0x31d, 0x31f,
											  0x321, 0x323, 0x325, 0x327, 0x329, 0x32b, 0x32d, 0x32f, 0x331, 0x333, 0x335, 0x337, 0x339, 0x33b, 0x33d, 0x33f};

// MAC address
u16		static_mac_1_start_addr		= 0x310;

u16		static_mac_2_start_addr		= 0x316;

u16		static_mac_3_start_addr		= 0x31c;

u16		static_mac_4_start_addr		= 0x322;

u16 	static_mac_5_start_addr		= 0x328;

u16 	static_mac_6_start_addr		= 0x32e;
//Hung

u16		static_mac_7_start_addr		= 0x334;

u16		static_mac_8_start_addr		= 0x33a;

u16		static_mac_9_start_addr		= 0x340;

u16		static_mac_10_start_addr	= 0x346;

u16 	static_mac_11_start_addr	= 0x34c;

u16 	static_mac_12_start_addr	= 0x352;




u16 	speed_port_1_addr 			= 0x360;
u16		speed_port_2_addr			= 0x366;
u16		speed_port_3_addr			= 0x36c;
u16		speed_port_4_addr			= 0x372;
//Hung
u16 	speed_port_5_addr 			= 0x378;
u16		speed_port_6_addr			= 0x37e;
u16		speed_port_7_addr			= 0x385;
u16		speed_port_8_addr			= 0x38b;
u16 	speed_port_9_addr 			= 0x391;
u16		speed_port_10_addr			= 0x397;
u16		speed_port_11_addr			= 0x39e;
u16		speed_port_12_addr			= 0x3a5;
