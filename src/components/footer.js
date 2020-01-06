import React from "react"
import UserLink from './userLinks'
import config from '../../data/siteConfig'
// import { OutboundLink } from 'gatsby-plugin-google-analytics'

const Footer = () => {
    return(
        <div><UserLink link={config.userLinks}/> 
        </div>
        
    )
}
export default Footer