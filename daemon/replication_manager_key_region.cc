// Copyright (c) 2011, Cornell University
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//     * Redistributions of source code must retain the above copyright notice,
//       this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of HyperDex nor the names of its contributors may be
//       used to endorse or promote products derived from this software without
//       specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

// HyperDex
#include "daemon/replication_manager_key_region.h"

using hyperdex::replication_manager;

replication_manager :: key_region :: key_region()
    : region()
    , key()
{
}

replication_manager :: key_region :: key_region(const region_id& r, const e::slice& k)
    : region(r)
    , key(k)
{
}

replication_manager :: key_region :: key_region(const key_region& other)
    : region(other.region)
    , key(other.key)
{
}

bool
replication_manager :: key_region :: operator < (const key_region& rhs) const
{
    if (region == rhs.region)
    {
        return key < rhs.key;
    }

    return region < rhs.region;
}

bool
replication_manager :: key_region :: operator == (const key_region& rhs) const
{
    return region == rhs.region && key == rhs.key;
}

replication_manager::key_region&
replication_manager :: key_region :: operator = (const key_region& rhs)
{
    if (this != &rhs)
    {
        region = rhs.region;
        key = rhs.key;
    }

    return *this;
}
