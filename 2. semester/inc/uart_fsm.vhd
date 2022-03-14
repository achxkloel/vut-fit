-- uart_fsm.vhd: UART controller - finite state machine
--
-- Project: INC - UART Receiver
-- Author:	Evgeny Torbin
-- Faculty:	Information Technology, Brno University of Technology
-- Date:	06.05.2020
--
library ieee;
use ieee.std_logic_1164.all;

-------------------------------------------------
entity UART_FSM is
port(

	-- Vstupni signaly --
    i_CLK      		: in std_logic; -- hodinovy signal
    i_RST      		: in std_logic; -- resetovaci signal
    i_DIN      		: in std_logic; -- input signal
    i_CLK_CNT		: in std_logic_vector(4 downto 0); -- pocitadlo hodnivych taktu
    i_BIT_CNT		: in std_logic_vector(3 downto 0); -- pocitadlo bitu
    
    -- Vystupni signaly --
    -- Pokud se rovna 1:
    o_RX_ENABLED	: out std_logic; -- nacitame data
    o_CNT_ENABLED	: out std_logic; -- pocitame takty hodinoveho signalu
    o_DATA_VALID 	: out std_logic  -- nastavime DOUT_VLD
    
);
end entity UART_FSM;

-------------------------------------------------
architecture behavioral of UART_FSM is

-- Stavy konecneho automatu (typ) --
type t_FSM_State is (
	s_Idle,
	s_RX_Start_Bit,
	s_RX_Data_Bits,
	s_RX_Stop_Bit
);

-- Definice stavu automatu --
signal c_state : t_FSM_State; -- soucasny stav
signal n_state : t_FSM_State; -- nasleduici stav

begin
    
    -- Registr soucasneho stavu --
    c_state_reg: process (i_CLK, i_RST) begin
    
    	-- Pokud resetovaci signal nastaven na 1,
    	-- prejdeme na pocatecni stav konecnecho automatu
    	if (i_RST = '1') then
    		c_state <= s_Idle;
    		
		-- Jinak
    	else
    	
    		-- Prepiseme soucasny stav nasledujicim
    		-- pri kazdem taktu hodinoveho signalu
    		if (i_CLK'event) and (i_CLK = '1') then
    			c_state <= n_state;
			end if;
			
    	end if;
    	
    end process;
    
    -- Logika nasledujiciho stavu --
    n_state_logic: process (c_state, i_DIN, i_CLK_CNT, i_BIT_CNT) begin
        case c_state is
        
        	-- Pocatecni stav --
        	-- Cekame na prechod z log. 1 na log. 0,
        	-- abychom detektovali START bit.
        	-- Tedy kdyz input signal se bude rovnat nule,
        	-- tak zmenime nasledujici stav na s_RX_Start_bit.
            when s_Idle =>
                if i_DIN = '0' then
                    n_state <= s_RX_Start_bit;
                end if;
                
            -- START bit --
            -- V tomto stavu cekame na zacatek datoveho bytu
            -- Od zacatku START bitu do jeho stredu - 8 taktu hodinoveho signalu
            -- Od stredu START bitu do stredu datoveho bitu - 16 taktu
            -- Proto musime pockat dohromady 24 taktu, abychom dostali do dat. bitu
            -- Zmena dvou stavu (do a po) zabira 2 takty,
            -- takze porovnavame i_CLK_CNT s cislem 22 (binarne)
            -- a kdyz se rovna, tak dostavame se do stavu s_RX_Data_Bits
            when s_RX_Start_Bit =>
                if i_CLK_CNT = "10110" then
                    n_state <= s_RX_Data_Bits;
                end if;
                
            -- Nacitani dat --
            -- V tomto stavu nacitame data
            -- Pokud pocet bytu bude se rovnat 8,
            -- tak zapiseme do soucasneho stavu nasledujici - s_RX_Stop_Bit
            when s_RX_Data_Bits =>
                if i_BIT_CNT = "1000" then
                    n_state <= s_RX_Stop_Bit;
                end if;
                
            -- STOP bit --
            -- Posledni stav - konec prenosu bajtu
            -- Pokud input signal bude se rovnat 1,
            -- tak hned (bez cekani 16 hodinovych taktu do stredu STOP bitu)
            -- menime nasledujici stav na pocatek
            when s_RX_Stop_Bit =>
                if i_DIN = '1' then
                    n_state <= s_Idle;
                end if;
                
            -- V ostatnich pripadech --
            when others => null;
            
        end case;
    end process;
    
    -- Logika pro generovani vystupu --
    output_logic: process (c_state) begin
    	
    	-- Defaultni nataveni vystupnich signalu
    	o_RX_ENABLED 	<= '0';
    	o_CNT_ENABLED 	<= '0';
    	o_DATA_VALID 	<= '0';
    	
    	-- Na zaklade soucasneho stavu nastavime vystupni signaly
    	case c_state is
    	
    		when s_RX_Start_Bit => 	o_CNT_ENABLED 	<= '1';
    		
    		when s_RX_Data_Bits => 	o_CNT_ENABLED 	<= '1';
    								o_RX_ENABLED 	<= '1';
    								
    		when s_Rx_Stop_Bit 	=>	o_DATA_VALID 	<= '1';
    		
    		when others 		=> 	null;
    		
		end case;
		
    end process;
    
end behavioral;
