//Author: Kevin Mc Gee, Based on original work(L4 routing) by Moab Rodrigues de Jesus
/*
// Network topology:
//
// n0 		n1 		n2 
// |		|		|  
// ===================
// <--------><------>
//	   25m		25m
// WSN (802.15.4)
*/

//Based on the examples wsn-ping6.cc, ping6.cc, example-ping-lr-wpan.cc and
//udp-echo.cc

#include "ns3/core-module.h"
#include "ns3/lr-wpan-module.h" //my stuff should be in here too!
#include "ns3/propagation-loss-model.h"
#include "ns3/propagation-delay-model.h"
#include "ns3/single-model-spectrum-channel.h"
#include "ns3/constant-position-mobility-model.h"
#include "ns3/applications-module.h"
#include "ns3/ipv6-static-routing-helper.h"
#include "ns3/ipv6-routing-table-entry.h"
#include "ns3/internet-stack-helper.h"
#include <ns3/ipv6-address-helper.h>
#include <ns3/sixlowpan-helper.h>
#include "ns3/internet-module.h"
#include "ns3/network-module.h"
#include <ns3/spectrum-value.h>
#include "ns3/netanim-module.h"
#include "ns3/mobility-module.h"
#include "ns3/flow-monitor-module.h"
#include "ns3/ipv6-flow-classifier.h"
#include "ns3/ping6-helper.h"
//#include "lowpanL2Hdr.h"
using namespace ns3;
NS_LOG_COMPONENT_DEFINE ("WSN");

//callback for packet reception
void ReceivePacket (Ptr<Socket> dest, Ptr<Socket> socket){
	//dest->Connect(remote);
	while (socket->Recv ()){
		NS_LOG_UNCOND ("Node " << socket->GetNode()->GetId() << " received one packet!");
		dest->Send (Create<Packet>());
	}
	//dest->Close();
}



void ReceiveDestiny (Ptr<Socket> socket){
	while (socket->Recv ()){
		NS_LOG_UNCOND ("Package arrived at no 3");
	}
}



static void GenerateTraffic (Ptr<Socket> socket, uint32_t pktSize, uint32_t pktCount, Time pktInterval ){
	if (pktCount > 0){
		socket->Send (Create<Packet> (pktSize));
		NS_LOG_UNCOND("Node 0 sends packet number "<< pktCount);
		Simulator::Schedule (pktInterval, &GenerateTraffic, socket, pktSize,pktCount-1, pktInterval);
	}else{
		socket->Close ();
	}
}



int main(){
// Enable calculation of FCS in the trailers. Only necessary when interacting with real devices or wireshark.(Tommaso Pecorella)
GlobalValue::Bind ("ChecksumEnabled", BooleanValue (true));

LogComponentEnable ("WSN", LOG_LEVEL_INFO);	
LogComponentEnable ("Socket", LOG_LEVEL_INFO);
lowpanL2Hdr myHeader;


//Create 3 nodes, and a device for each one
Ptr<Node> n0 = CreateObject<Node>();
Ptr<Node> n1 = CreateObject<Node>();
Ptr<Node> n2 = CreateObject<Node>();

//deprecated stuff for channel tuning
double txPower = 0;
uint32_t channelNumber = 11;

//add the nodes to a container
NodeContainer nodes (n0, n1, n2);

//create several LrWpanNetDevice pointers
Ptr<LrWpanNetDevice> dev0 = CreateObject<LrWpanNetDevice>();
Ptr<LrWpanNetDevice> dev1 = CreateObject<LrWpanNetDevice>();
Ptr<LrWpanNetDevice> dev2 = CreateObject<LrWpanNetDevice>();



//NetDeviceContainer lrwpanDevices = NetDeviceContainer(dev0, dev1, dev2, dev3);
LrWpanHelper lrWpanHelper; //use helper later on to enable Layer 2 PCAP traces
//create a device container for the lrWpanNetDevice nodes
NetDeviceContainer lrwpanDevices;

lrwpanDevices.Add(dev0);
lrwpanDevices.Add(dev1);
lrwpanDevices.Add(dev2);

//std::cout<<lrwpanDevices.GetN();
//assign short MAC addresses to each of the LrWpanNetDevices
dev0->SetAddress(Mac16Address("00:01"));
dev1->SetAddress(Mac16Address("00:02"));
dev2->SetAddress(Mac16Address("00:03"));


///Current attempt at channel design - Needs overhaul
//Each device must be attached to same channel - 
Ptr<SingleModelSpectrumChannel> channel = CreateObject<SingleModelSpectrumChannel>(); //maybe not the way to go. Look at 
// https://www.nsnam.org/doxygen/lr-wpan-error-distance-plot_8cc_source.html
Ptr<FriisPropagationLossModel> propModel = CreateObject<FriisPropagationLossModel>();//just for now
Ptr<ConstantSpeedPropagationDelayModel> delayModel = CreateObject<ConstantSpeedPropagationDelayModel>();//not sure what else to do
channel->AddPropagationLossModel(propModel);
channel->SetPropagationDelayModel(delayModel);

//set each device to have these channel characteristics
dev0->SetChannel(channel);
dev1->SetChannel(channel);
dev2->SetChannel(channel);

//To complete configuration, a LrWpanNetDevice must be added to a node
n0->AddDevice(dev0);
n1->AddDevice(dev1);
n2->AddDevice(dev2);

//I wish to check out what the deal is with the current error model:
Ptr<LrWpanErrorModel> newModel = CreateObject<LrWpanErrorModel>();
Ptr<LrWpanPhy> existingPhy = dev0->GetPhy();
existingPhy->SetErrorModel(newModel);

Ptr<ConstantPositionMobilityModel> sender0 = CreateObject<ConstantPositionMobilityModel>();
//sender0->SetPosition(Vector(0,0,0));
sender0->SetPosition(Vector(0,0,0));
dev0->GetPhy()->SetMobility(sender0);

Ptr<ConstantPositionMobilityModel> sender1 = CreateObject<ConstantPositionMobilityModel>();
//sender1->SetPosition(Vector(5,0,0));
sender1->SetPosition(Vector(25,0,0));
dev1->GetPhy()->SetMobility(sender1);

Ptr<ConstantPositionMobilityModel> sender2 = CreateObject<ConstantPositionMobilityModel>();
//sender2->SetPosition(Vector(10,0,0));
sender2->SetPosition(Vector(50,0,0));
dev2->GetPhy()->SetMobility(sender2);





//configure power of transmission - Needs overhaul
LrWpanSpectrumValueHelper svh;
Ptr<SpectrumValue> psd = svh.CreateTxPowerSpectralDensity (txPower, channelNumber);
//assign to the devices
dev0->GetPhy ()->SetTxPowerSpectralDensity (psd);
dev1->GetPhy ()->SetTxPowerSpectralDensity (psd);
dev2->GetPhy ()->SetTxPowerSpectralDensity (psd);

//set the drop trace callback for Rx:


//Install protocol stack at the nodes
InternetStackHelper internetv6;
internetv6.Install (nodes);
//install the 6LoWPAN element to the nodes
SixLowPanHelper sixlowpan;
NetDeviceContainer devices = sixlowpan.Install (lrwpanDevices);

//set IPv6 addresses:
Ipv6AddressHelper ipv6;
ipv6.SetBase (Ipv6Address ("2001:2::"), Ipv6Prefix (64));

Ipv6InterfaceContainer deviceInterfaces;
deviceInterfaces = ipv6.Assign (devices);
//allow each of the devices to act as a router
deviceInterfaces.SetForwarding (0, true);
deviceInterfaces.SetForwarding (1, true);
deviceInterfaces.SetForwarding (2, true);
deviceInterfaces.SetDefaultRouteInAllNodes (0); //I forget what this is for

//just print out the assigned addresses for reference
Address address0, address1, address2, address3; //This is necessary for configure correctly the app layer.
address0 = Address(deviceInterfaces.GetAddress (0,1));
address1 = Address(deviceInterfaces.GetAddress (1,1));
address2 = Address(deviceInterfaces.GetAddress (2,1));
//Check the address of the nodes
std::cout<<address0<<std::endl;
std::cout<<address1<<std::endl;
std::cout<<address2<<std::endl;

///Static routing stuff here
Ptr<Ipv6> ipv6_0 = n0->GetObject<Ipv6> ();
Ptr<Ipv6> ipv6_1 = n1->GetObject<Ipv6> ();
Ptr<Ipv6> ipv6_2 = n2->GetObject<Ipv6> ();

uint32_t one = 1; //Interface number
//uint32_t two = 2;
//Add static routing
NS_LOG_INFO ("Creating Static Routes.");
Ipv6StaticRoutingHelper ipv6RoutingHelper;

Ptr<Ipv6StaticRouting> staticRouting1 = ipv6RoutingHelper.GetStaticRouting(ipv6_0);
//OLD: staticRouting1->AddHostRouteTo (deviceInterfaces.GetAddress (3,1), deviceInterfaces.GetAddress (1,1), 1);
staticRouting1->AddHostRouteTo (deviceInterfaces.GetAddress (2,1), deviceInterfaces.GetAddress (1,1) ,one, Ipv6Address ("2001:2::"),1);

Ptr<Ipv6StaticRouting> staticRouting2 = ipv6RoutingHelper.GetStaticRouting(ipv6_1);
staticRouting2->AddHostRouteTo (deviceInterfaces.GetAddress (2,1), deviceInterfaces.GetAddress (2,1) ,one, Ipv6Address ("2001:2::"),2); //forward entry
staticRouting2->AddHostRouteTo (deviceInterfaces.GetAddress (0,1),deviceInterfaces.GetAddress (0,1), 1); //backward entry?

Ptr<Ipv6StaticRouting> staticRouting3 = ipv6RoutingHelper.GetStaticRouting(ipv6_2);

staticRouting3->AddHostRouteTo (deviceInterfaces.GetAddress (1,1), deviceInterfaces.GetAddress (1,1), 1); //backward entry 1?
staticRouting3->AddHostRouteTo (deviceInterfaces.GetAddress (0,1), deviceInterfaces.GetAddress (1,1) ,one, Ipv6Address ("2001:2::"),1); //backward entry 2?





///Add app layer UDP
NS_LOG_INFO ("Adding PING layer.");
uint32_t packetSize = 1200; //bytes
Time interPacketInterval = Seconds (4);//Seconds (0.1);
uint32_t numPackets = 15; // packets
TypeId tid = TypeId::LookupByName ("ns3::UdpSocketFactory");
Ping6Helper ping6_SRC;
ping6_SRC.SetLocal (deviceInterfaces.GetAddress (0, 1));
ping6_SRC.SetRemote (deviceInterfaces.GetAddress (2, 1));
ping6_SRC.SetAttribute ("MaxPackets", UintegerValue (numPackets));
ping6_SRC.SetAttribute ("Interval", TimeValue (interPacketInterval));
ping6_SRC.SetAttribute ("PacketSize", UintegerValue (packetSize));
ApplicationContainer apps_SRC = ping6_SRC.Install (nodes.Get (0));
apps_SRC.Start (Seconds (2.0));
apps_SRC.Stop (Seconds (80.0));

///PCAP traces:
//Layer 3 PCAP
internetv6.EnablePcapIpv6All(std::string ("wsn_rOver_V6_PCAP-1200Bytes"));
//Layer 2 PCAP
lrWpanHelper.EnablePcap(std::string ("wsn_rOverL2_PCAP-1200Bytes"),lrwpanDevices, false);

//Mobility - Node location
MobilityHelper mobility;

Ptr<ListPositionAllocator> positionAlloc = CreateObject
<ListPositionAllocator>();
mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");

mobility.Install(nodes);
///Some display stuff
AnimationInterface anim ("wsn_main.xml"); 
anim.EnablePacketMetadata(true);


//run the simulation:
NS_LOG_INFO ("Run Simulation.");
//Not UDP!!!
//Simulator::ScheduleWithContext (source->GetNode ()->GetId (), Seconds (1.0), &GenerateTraffic, source, packetSize, numPackets, interPacketInterval);


// Calculate Throughput using Flowmonitor
FlowMonitorHelper flowmon;
Ptr<FlowMonitor> monitor = flowmon.InstallAll();

// Now, do the actual simulation.
Simulator::Stop (Seconds(100.0));
Simulator::Run ();

//stats:
monitor->CheckForLostPackets ();
Ptr<Ipv6FlowClassifier> classifier = DynamicCast<Ipv6FlowClassifier>(flowmon.GetClassifier ());
//std::map<FlowId, FlowMonitor::FlowStats> stats = monitor->GetFlowStats ();
//std::map<FlowId, FlowMonitor::FlowStats>::const_iterator i = stats.begin();


//print out of results:
//std::cout << "Flow IP : (" << address0 << " -> " << address2 <<")\n";
//std::cout << " Tx Bytes: " << i->second.txBytes << "\n";
//std::cout << " Rx Bytes: " << i->second.rxBytes << "\n";
//std::cout << " Throughput: " << i->second.rxBytes * 8.0 /(i->second.timeLastRxPacket.GetSeconds() - i->second.timeFirstTxPacket.GetSeconds())/1024/1024 << " Mbps\n";

//output:
monitor->SerializeToXmlFile("wsn_main.flowmon", true, true);


//End:
Simulator::Destroy ();
NS_LOG_INFO ("Done.");
return 0;
}
