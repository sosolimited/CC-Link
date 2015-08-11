//
//  memoryUtilities.h
//  emptyExample
//
//  Created by Sosolimited on 8/11/15.
//
//

#pragma once

// Templated make_shared function
template< typename T >
std::shared_ptr< T > make_shared() {
	
	return std::shared_ptr< T >( new T() );
}