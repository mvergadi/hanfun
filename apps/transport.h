// =============================================================================
/*!
 * \file       /HAN-FUN/apps/transport.h
 *
 * This file contains the definitions of the example transport layer.
 *
 * \author     Filipe Alves <filipe.alves@bithium.com>
 *
 * \version    x.x.x
 *
 * \copyright  Copyright &copy; &nbsp; 2014 Bithium S.A.
 */
// =============================================================================

#ifndef HF_APP_TRANSPORT_H
#define HF_APP_TRANSPORT_H

#include <forward_list>

#include "hanfun.h"

#include "base.h"

class Link:public HF::Transport::Link
{
   HF::Transport::Layer    * tsp_layer;
   HF::Transport::Endpoint * dst;

   HF::UID * _uid;

   uint16_t _address;

   public :

   Link * remote;

   Link(HF::Transport::Layer *tsp_layer, HF::Transport::Endpoint * dst, HF::UID * uid) :
      tsp_layer (tsp_layer), dst (dst), _uid(uid)
   {}

   Link(HF::Transport::Layer *tsp_layer, Base * dst) :
      tsp_layer (tsp_layer), dst (dst), _uid(dst->uid())
   {}

   void send(HF::Protocol::Packet &packet)
   {
      cout << __PRETTY_FUNCTION__ << endl;

      HF::Serializable * data = packet.message.payload;

      size_t size = ( data != nullptr ? data->size() : 0);
      HF::ByteArray payload(size);

      if( data != nullptr)
      {
         data->pack(payload, 0);
         packet.message.length = size;
      }

      if( dst != nullptr )
      {
         packet.link = remote;
         dst->receive(packet, payload, 0);
      }
   }

   HF::Transport::Layer * transport()
   {
      return tsp_layer;
   }

   HF::UID * uid()
   {
      return _uid;
   }

   uint16_t address () const
   {
      return _address;
   }

   /*!
    * Set the address of this link corresponds to.
    *
    * @param [in] addr  the address value to set.
    */
   void address (uint16_t addr)
   {
      _address = addr;
   }
};

class Transport:public HF::Transport::Layer
{
   forward_list<HF::Transport::Endpoint *> endpoints;

   forward_list<Link *> links;

   Base * base;

   const HF::UID * _uid;

   public :

   Transport() : base(nullptr), _uid(new HF::UID())
   {}

   virtual ~Transport()
   {
      delete _uid;
   }

   void initialize ()
   {}

   void initialize (Base * base)
   {
      this->base = base;
   }

   void add (HF::Transport::Endpoint *ep, HF::UID * uid)
   {
      add(ep);

      Link * link_base = new Link(this, base);
      links.push_front(link_base);
      ep->connected(link_base);

      Link * link_device = new Link(this, ep, uid);
      links.push_front(link_device);
      base->connected(link_device);

      link_base->remote = link_device;
      link_device->remote = link_base;
   }

   void remove (HF::Transport::Endpoint *ep = nullptr)
   {
      if( ep != nullptr)
      {
         endpoints.remove(ep);
      }
      else
      {
         endpoints.clear();
      }
   }

   void destroy ()
   {
      remove(nullptr);
   }

   const HF::UID * uid () const
   {
      return _uid;
   }

   private :

   void add (HF::Transport::Endpoint *ep)
   {
      endpoints.push_front(ep);
   }
};

#endif /* HF_APP_TRANSPORT_H */