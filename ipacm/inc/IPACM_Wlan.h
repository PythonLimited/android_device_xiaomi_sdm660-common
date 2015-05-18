/*
Copyright (c) 2013, The Linux Foundation. All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above
      copyright notice, this list of conditions and the following
      disclaimer in the documentation and/or other materials provided
      with the distribution.
    * Neither the name of The Linux Foundation nor the names of its
      contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
/*!
		@file
		IPACM_Wlan.h

		@brief
		This file implements the WLAN iface functionality.

		@Author
		Skylar Chang

*/
#ifndef IPACM_WLAN_H
#define IPACM_WLAN_H

#include <stdio.h>
#include <IPACM_CmdQueue.h>
#include <linux/msm_ipa.h>
#include "IPACM_Routing.h"
#include "IPACM_Filtering.h"
#include "IPACM_Lan.h"
#include "IPACM_Iface.h"
#include "IPACM_Conntrack_NATApp.h"

typedef struct _wlan_client_rt_hdl
{
	uint32_t wifi_rt_rule_hdl_v4;
	uint32_t wifi_rt_rule_hdl_v6[IPV6_NUM_ADDR];
	uint32_t wifi_rt_rule_hdl_v6_wan[IPV6_NUM_ADDR];
}wlan_client_rt_hdl;

typedef struct _ipa_wlan_client
{
	ipacm_event_data_wlan_ex* p_hdr_info;
	uint8_t mac[IPA_MAC_ADDR_SIZE];
	uint32_t v4_addr;
	uint32_t v6_addr[IPV6_NUM_ADDR][4];
	uint32_t hdr_hdl_v4;
	uint32_t hdr_hdl_v6;
	bool route_rule_set_v4;
	int route_rule_set_v6;
	bool ipv4_set;
	int ipv6_set;
	bool ipv4_header_set;
	bool ipv6_header_set;
	bool power_save_set;
	wlan_client_rt_hdl wifi_rt_hdl[0]; /* depends on number of tx properties */
}ipa_wlan_client;

/* wlan iface */
class IPACM_Wlan : public IPACM_Lan
{

public:

	IPACM_Wlan(int iface_index);
	virtual ~IPACM_Wlan(void);

	static int total_num_wifi_clients;

	void event_callback(ipa_cm_event_id event,
											void *data);

	virtual int add_lan2lan_hdr(ipa_ip_type iptype, uint8_t* src_mac, uint8_t* dst_mac, uint32_t* hdr_hdl);

private:

	eth_bridge_client_flt_info eth_bridge_lan_client_flt_info[IPA_LAN_TO_LAN_MAX_LAN_CLIENT];
	int lan_client_flt_info_count;

	uint32_t wlan_guest_ap_flt_rule_hdl_v4[IPA_MAX_PRIVATE_SUBNET_ENTRIES];

	uint32_t wlan_guest_ap_flt_rule_hdl_v6[IPA_GUEST_AP_IPv6_FLT_RULE_ENTRIES];

	static lan2lan_flt_rule_hdl self_client_flt_rule_hdl_v4[IPA_LAN_TO_LAN_MAX_WLAN_CLIENT];
	static lan2lan_flt_rule_hdl self_client_flt_rule_hdl_v6[IPA_LAN_TO_LAN_MAX_WLAN_CLIENT];

	static lan2lan_flt_rule_hdl lan_client_flt_rule_hdl_v4[IPA_LAN_TO_LAN_MAX_LAN_CLIENT];
	static lan2lan_flt_rule_hdl lan_client_flt_rule_hdl_v6[IPA_LAN_TO_LAN_MAX_LAN_CLIENT];

	bool is_guest_ap;

	eth_bridge_client_rt_info* eth_bridge_wlan_client_rt_from_lan_info_v4;
	int wlan_client_rt_from_lan_info_count_v4;
	eth_bridge_client_rt_info* eth_bridge_wlan_client_rt_from_lan_info_v6;
	int wlan_client_rt_from_lan_info_count_v6;

	eth_bridge_client_rt_info* eth_bridge_wlan_client_rt_from_wlan_info_v4;
	int wlan_client_rt_from_wlan_info_count_v4;
	eth_bridge_client_rt_info* eth_bridge_wlan_client_rt_from_wlan_info_v6;
	int wlan_client_rt_from_wlan_info_count_v6;

	virtual int eth_bridge_add_wlan_guest_ap_flt_rule(ipa_ip_type iptype);

	int eth_bridge_install_wlan_guest_ap_ipv6_flt_rule();

	virtual int eth_bridge_handle_dummy_wlan_client_flt_rule(ipa_ip_type iptype);

	virtual int eth_bridge_handle_dummy_lan_client_flt_rule(ipa_ip_type iptype);

	int eth_bridge_add_lan_client_flt_rule(uint8_t* mac, ipa_ip_type iptype);

	int eth_bridge_del_lan_client_flt_rule(uint8_t* mac);

	int eth_bridge_add_self_client_flt_rule(uint8_t* mac, ipa_ip_type iptype);

	int eth_bridge_del_self_client_flt_rule(uint8_t* mac);

	virtual int eth_bridge_install_cache_wlan_client_flt_rule(ipa_ip_type iptype);

	virtual int eth_bridge_install_cache_lan_client_flt_rule(ipa_ip_type iptype);

	int eth_bridge_add_wlan_client_rt_rule(uint8_t* mac, eth_bridge_src_iface src, ipa_ip_type iptype);

	int eth_bridge_del_wlan_client_rt_rule(uint8_t* mac, eth_bridge_src_iface src);

	eth_bridge_client_rt_info* eth_bridge_get_client_rt_info_ptr(uint8_t index, eth_bridge_src_iface src, ipa_ip_type iptype);

	void eth_bridge_add_wlan_client(uint8_t* mac, int if_num);

	void eth_bridge_del_wlan_client(uint8_t* mac);


	int wlan_client_len;
	ipa_wlan_client *wlan_client;

	int header_name_count;
	int num_wifi_client;

	int wlan_ap_index;

	static uint32_t* dummy_flt_rule_hdl_v4;
	static uint32_t* dummy_flt_rule_hdl_v6;

	static int num_wlan_ap_iface;

	NatApp *Nat_App;

	inline ipa_wlan_client* get_client_memptr(ipa_wlan_client *param, int cnt)
	{
	    char *ret = ((char *)param) + (wlan_client_len * cnt);
		return (ipa_wlan_client *)ret;
	}

	inline int get_wlan_client_index(uint8_t *mac_addr)
	{
		int cnt;
		int num_wifi_client_tmp = num_wifi_client;

		IPACMDBG_H("Passed MAC %02x:%02x:%02x:%02x:%02x:%02x\n",
						 mac_addr[0], mac_addr[1], mac_addr[2],
						 mac_addr[3], mac_addr[4], mac_addr[5]);

		for(cnt = 0; cnt < num_wifi_client_tmp; cnt++)
		{
			IPACMDBG_H("stored MAC %02x:%02x:%02x:%02x:%02x:%02x\n",
							 get_client_memptr(wlan_client, cnt)->mac[0],
							 get_client_memptr(wlan_client, cnt)->mac[1],
							 get_client_memptr(wlan_client, cnt)->mac[2],
							 get_client_memptr(wlan_client, cnt)->mac[3],
							 get_client_memptr(wlan_client, cnt)->mac[4],
							 get_client_memptr(wlan_client, cnt)->mac[5]);

			if(memcmp(get_client_memptr(wlan_client, cnt)->mac,
								mac_addr,
								sizeof(get_client_memptr(wlan_client, cnt)->mac)) == 0)
			{
				IPACMDBG_H("Matched client index: %d\n", cnt);
				return cnt;
			}
		}

		return IPACM_INVALID_INDEX;
	}

	inline int delete_default_qos_rtrules(int clt_indx, ipa_ip_type iptype)
	{
		uint32_t tx_index;
		uint32_t rt_hdl;
		int num_v6;

		if(iptype == IPA_IP_v4)
		{
		     for(tx_index = 0; tx_index < iface_query->num_tx_props; tx_index++)
		     {
		        if((tx_prop->tx[tx_index].ip == IPA_IP_v4) && (get_client_memptr(wlan_client, clt_indx)->route_rule_set_v4==true)) /* for ipv4 */
			{
				IPACMDBG_H("Delete client index %d ipv4 Qos rules for tx:%d \n",clt_indx,tx_index);
				rt_hdl = get_client_memptr(wlan_client, clt_indx)->wifi_rt_hdl[tx_index].wifi_rt_rule_hdl_v4;

				if(m_routing.DeleteRoutingHdl(rt_hdl, IPA_IP_v4) == false)
				{
					return IPACM_FAILURE;
				}
			}
		     } /* end of for loop */

		     /* clean the 4 Qos ipv4 RT rules for client:clt_indx */
		     if(get_client_memptr(wlan_client, clt_indx)->route_rule_set_v4==true) /* for ipv4 */
		     {
				get_client_memptr(wlan_client, clt_indx)->route_rule_set_v4 = false;
		     }
		}

		if(iptype == IPA_IP_v6)
		{
		    for(tx_index = 0; tx_index < iface_query->num_tx_props; tx_index++)
		    {

				if((tx_prop->tx[tx_index].ip == IPA_IP_v6) && (get_client_memptr(wlan_client, clt_indx)->route_rule_set_v6 != 0)) /* for ipv6 */
				{
					for(num_v6 =0;num_v6 < get_client_memptr(wlan_client, clt_indx)->route_rule_set_v6;num_v6++)
					{
						IPACMDBG_H("Delete client index %d ipv6 Qos rules for %d-st ipv6 for tx:%d\n", clt_indx,num_v6,tx_index);
						rt_hdl = get_client_memptr(wlan_client, clt_indx)->wifi_rt_hdl[tx_index].wifi_rt_rule_hdl_v6[num_v6];
						if(m_routing.DeleteRoutingHdl(rt_hdl, IPA_IP_v6) == false)
						{
							return IPACM_FAILURE;
						}

						rt_hdl = get_client_memptr(wlan_client, clt_indx)->wifi_rt_hdl[tx_index].wifi_rt_rule_hdl_v6_wan[num_v6];
						if(m_routing.DeleteRoutingHdl(rt_hdl, IPA_IP_v6) == false)
						{
							return IPACM_FAILURE;
						}
					}

				}
			} /* end of for loop */

		    /* clean the 4 Qos ipv6 RT rules for client:clt_indx */
		    if(get_client_memptr(wlan_client, clt_indx)->route_rule_set_v6 != 0) /* for ipv6 */
		    {
		                 get_client_memptr(wlan_client, clt_indx)->route_rule_set_v6 = 0;
                    }
		}

		return IPACM_SUCCESS;
	}

	/* for handle wifi client initial,copy all partial headers (tx property) */
	int handle_wlan_client_init_ex(ipacm_event_data_wlan_ex *data);

	/*handle lan2lan internal mesg posting*/
	int handle_lan2lan_msg_post(uint8_t *mac_addr, ipa_cm_event_id event, ipa_ip_type iptype);

	/*handle wifi client */
	int handle_wlan_client_ipaddr(ipacm_event_data_all *data);

	/*handle wifi client routing rule*/
	int handle_wlan_client_route_rule(uint8_t *mac_addr, ipa_ip_type iptype);

	/*handle wifi client power-save mode*/
	int handle_wlan_client_pwrsave(uint8_t *mac_addr);

	/*handle wifi client del mode*/
	int handle_wlan_client_down_evt(uint8_t *mac_addr);

	/*handle wlan iface down event*/
	int handle_down_evt();

	/* add dummy filtering rules for WLAN AP-AP mode support */
	void add_dummy_flt_rule();

	/* install dummy filtering rules for WLAN AP-AP mode support */
	int install_dummy_flt_rule(ipa_ip_type iptype, int num_rule);

	/* delete dummy flt rule for WLAN AP-AP mode support*/
	void del_dummy_flt_rule();

	/*Configure the initial filter rules */
	virtual int init_fl_rule(ipa_ip_type iptype);

	virtual int add_dummy_lan2lan_flt_rule(ipa_ip_type iptype);

	virtual int add_dummy_private_subnet_flt_rule(ipa_ip_type iptype);

	/*configure private subnet filter rules*/
	virtual int handle_private_subnet(ipa_ip_type iptype);

	/* install UL filter rule from Q6 */
	virtual int handle_uplink_filter_rule(ipacm_ext_prop* prop, ipa_ip_type iptype, uint8_t xlat_mux_id);

	/* install TCP control filter rules */
	virtual void install_tcp_ctl_flt_rule(ipa_ip_type iptype);

	/*handle reset wifi-client rt-rules */
	int handle_wlan_client_reset_rt(ipa_ip_type iptype);

	void handle_SCC_MCC_switch(ipa_ip_type);

	void eth_bridge_handle_wlan_SCC_MCC_switch(ipa_ip_type iptype);

	int eth_bridge_modify_wlan_client_flt_rule(uint8_t* mac, eth_bridge_dst_iface dst_iface, ipa_ip_type iptype);

	int eth_bridge_modify_wlan_rt_rule(uint8_t* mac, eth_bridge_src_iface src_iface, ipa_ip_type iptyp);

	/*handle wlan access mode switch */
	void eth_bridge_handle_wlan_mode_switch();

};


#endif /* IPACM_WLAN_H */
